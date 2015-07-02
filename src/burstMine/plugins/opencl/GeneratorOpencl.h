/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_PLUGINS_OPENCL_GENERATOR_OPENCL_H
#define CRYO_BURST_MINE_PLUGINS_OPENCL_GENERATOR_OPENCL_H

#include <memory>
#include <vector>
#include <exception>
#include <CL/CL.h>
#include <burstMine/ScoopsBuffer.h>
#include <burstMine/Generator.h>

#include "../../../opencl/OpenclPlatform.h"
#include "../../../opencl/OpenclDevice.h"
#include "../../../opencl/OpenclContext.h"
#include "../../../opencl/OpenclCommandQueue.h"
#include "../../../opencl/OpenclBuffer.h"
#include "../../../opencl/OpenclProgram.h"
#include "../../../opencl/OpenclKernel.h"
#include "GeneratorOpenclConfig.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {

class GeneratorOpencl : public cryo::burstMine::Generator {
	private:
		GeneratorOpenclConfig m_config;
		std::shared_ptr<cryo::opencl::OpenclPlatform> m_platform;
		std::shared_ptr<cryo::opencl::OpenclDevice> m_device;
		std::shared_ptr<cryo::opencl::OpenclContext> m_context;
		std::shared_ptr<cryo::opencl::OpenclCommandQueue> m_commandQueue;
		std::shared_ptr<cryo::opencl::OpenclBuffer> m_buffer;
		std::shared_ptr<cryo::opencl::OpenclProgram> m_program;
		std::vector<std::shared_ptr<cryo::opencl::OpenclKernel>> m_kernels;

	public:
		GeneratorOpencl(const GeneratorOpenclConfig& p_config) throw (std::exception, cryo::opencl::OpenclError);
		GeneratorOpencl(const GeneratorOpencl& p_other);
		virtual ~GeneratorOpencl();

		GeneratorOpencl& operator=(const GeneratorOpencl& p_other);

		inline const GeneratorOpenclConfig& getConfig() const;
		inline const cryo::opencl::OpenclPlatform& getPlatform() const;
		inline const cryo::opencl::OpenclDevice& getDevice() const;

		virtual unsigned int getMaxWorkSize() const;
		virtual void computePlots(const GenerationWork& p_work, cryo::burstMine::ScoopsBuffer& p_buffer) throw (std::exception, cryo::opencl::OpenclError);
};

}}}}

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {

const GeneratorOpenclConfig& GeneratorOpencl::getConfig() const {
	return m_config;
}

const cryo::opencl::OpenclPlatform& GeneratorOpencl::getPlatform() const {
	return *m_platform;
}

const cryo::opencl::OpenclDevice& GeneratorOpencl::getDevice() const {
	return *m_device;
}

}}}}

#endif
