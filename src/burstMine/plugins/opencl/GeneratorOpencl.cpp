/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include <memory>
#include <vector>
#include <stdexcept>

#include "../../../opencl/OpenclError.h"
#include "constants.h"
#include "GeneratorOpencl.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {

GeneratorOpencl::GeneratorOpencl(const GeneratorOpenclConfig& p_config) throw (std::exception, cryo::opencl::OpenclError)
: m_config(p_config) {
	m_config.normalize();
	if(m_config.getGlobalWorkSize() == 0) {
		throw std::runtime_error("Global work size can't be null");
	} else if(m_config.getLocalWorkSize() == 0) {
		throw std::runtime_error("Local work size can't be null");
	}

	std::vector<cryo::opencl::OpenclPlatform> platforms(cryo::opencl::OpenclPlatform::list());
	if(m_config.getPlatform() >= platforms.size()) {
		throw std::runtime_error("Invalid platform index");
	}

	m_platform = std::shared_ptr<cryo::opencl::OpenclPlatform>(new cryo::opencl::OpenclPlatform(platforms[m_config.getPlatform()]));

	std::vector<cryo::opencl::OpenclDevice> devices(cryo::opencl::OpenclDevice::list(*m_platform));
	if(m_config.getDevice() >= devices.size()) {
		throw std::runtime_error("Invalid device index");
	}

	m_device = std::shared_ptr<cryo::opencl::OpenclDevice>(new cryo::opencl::OpenclDevice(devices[m_config.getDevice()]));
	m_context = cryo::opencl::OpenclContext::create(*m_device);
	m_commandQueue = m_context->createCommandQueue(*m_device);
	m_buffer = m_context->createBuffer(cryo::opencl::OpenclMemFlag::ReadWrite, m_config.getGlobalWorkSize() * GEN_SIZE);
	m_program = m_context->createProgram(*m_device, m_config.getKernelsPath() + "/nonce.cl", m_config.getKernelsPath());

	m_kernels.push_back(m_program->createKernel("nonce_step1"));
	m_kernels.push_back(m_program->createKernel("nonce_step2"));
	m_kernels.push_back(m_program->createKernel("nonce_step3"));

	m_kernels[0]->setArgument(0, sizeof(cl_mem), (void*)&m_buffer->getHandle());
	m_kernels[1]->setArgument(0, sizeof(cl_mem), (void*)&m_buffer->getHandle());
	m_kernels[2]->setArgument(0, sizeof(cl_mem), (void*)&m_buffer->getHandle());
}

GeneratorOpencl::GeneratorOpencl(const GeneratorOpencl& p_other)
: m_config(p_other.m_config), m_platform(p_other.m_platform), m_device(p_other.m_device), m_context(p_other.m_context), m_commandQueue(p_other.m_commandQueue), m_buffer(p_other.m_buffer), m_program(p_other.m_program), m_kernels(p_other.m_kernels) {
}

GeneratorOpencl::~GeneratorOpencl() {
}

GeneratorOpencl& GeneratorOpencl::operator=(const GeneratorOpencl& p_other) {
	m_config = p_other.m_config;
	m_platform = p_other.m_platform;
	m_device = p_other.m_device;
	m_context = p_other.m_context;
	m_commandQueue = p_other.m_commandQueue;
	m_buffer = p_other.m_buffer;
	m_program = p_other.m_program;
	m_kernels = p_other.m_kernels;

	return *this;
}

unsigned int GeneratorOpencl::getMaxWorkSize() const {
	return static_cast<unsigned int>(m_config.getGlobalWorkSize());
}

void GeneratorOpencl::computePlots(const GenerationWork& p_work, cryo::burstMine::ScoopsBuffer& p_buffer) throw (std::exception, cryo::opencl::OpenclError) {
	if(p_work.getSize() > m_config.getGlobalWorkSize()) {
		throw std::runtime_error("Global work size too low for the requested work size");
	} else if(p_buffer.getSize() < p_work.getSize() * cryo::burstMine::SCOOPS_PER_PLOT) {
		throw std::runtime_error("Buffer size too low for the requested work size");
	}

	unsigned long long address = p_work.getAddress();
	unsigned long long offset = p_work.getOffset();
	unsigned int workSize = p_work.getSize();
	m_kernels[0]->setArgument(1, sizeof(unsigned int), static_cast<void*>(&workSize));
	m_kernels[0]->setArgument(2, sizeof(unsigned long long), static_cast<void*>(&address));
	m_kernels[0]->setArgument(3, sizeof(unsigned long long), static_cast<void*>(&offset));

	m_commandQueue->enqueueNDRangeKernel(*m_kernels[0], m_config.getGlobalWorkSize(), m_config.getLocalWorkSize());

	unsigned int hashesNumber = m_config.getHashesNumber();
	unsigned int hashesSize = hashesNumber * cryo::burstMine::HASH_SIZE;
	for(unsigned int i = 0 ; i < cryo::burstMine::PLOT_SIZE ; i += hashesSize) {
		unsigned int hashesOffset = cryo::burstMine::PLOT_SIZE - i;

		m_kernels[1]->setArgument(1, sizeof(unsigned int), static_cast<void*>(&workSize));
		m_kernels[1]->setArgument(2, sizeof(unsigned long long), static_cast<void*>(&offset));
		m_kernels[1]->setArgument(3, sizeof(unsigned int), static_cast<void*>(&hashesOffset));
		m_kernels[1]->setArgument(4, sizeof(unsigned int), static_cast<void*>(&hashesNumber));

		m_commandQueue->enqueueNDRangeKernel(*m_kernels[1], m_config.getGlobalWorkSize(), m_config.getLocalWorkSize());
//		m_commandQueue->finish();
	}

	m_kernels[2]->setArgument(1, sizeof(unsigned int), static_cast<void*>(&workSize));

	m_commandQueue->enqueueNDRangeKernel(*m_kernels[2], m_config.getGlobalWorkSize(), m_config.getLocalWorkSize());
// useful???
//	m_commandQueue->finish();

// TEST
m_commandQueue->finish();
// TEST

	std::size_t offsetGpu = 0;
	std::size_t offsetCpu = 0;
	for(unsigned int i = 0 ; i < p_work.getSize() ; ++i, offsetGpu += GEN_SIZE, offsetCpu += SCOOPS_PER_PLOT) {
		void* bufferPtr = static_cast<void*>(&p_buffer[offsetCpu]);
		m_commandQueue->enqueueReadBuffer(*m_buffer, sizeof(unsigned char) * offsetGpu, sizeof(unsigned char) * PLOT_SIZE, static_cast<unsigned char*>(bufferPtr));
	}

// TEST
m_commandQueue->finish();
// TEST
}

}}}}
