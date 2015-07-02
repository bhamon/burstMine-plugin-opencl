/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include <algorithm>

#include "GeneratorOpenclConfig.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {

GeneratorOpenclConfig::GeneratorOpenclConfig(std::size_t p_platform, std::size_t p_device, std::size_t p_globalWorkSize, std::size_t p_localWorkSize, unsigned int p_hashesNumber, const std::string p_kernelsPath)
: m_platform(p_platform), m_device(p_device), m_globalWorkSize(p_globalWorkSize), m_localWorkSize(p_localWorkSize), m_hashesNumber(p_hashesNumber), m_kernelsPath(p_kernelsPath) {
}

GeneratorOpenclConfig::GeneratorOpenclConfig(const GeneratorOpenclConfig& p_other)
: m_platform(p_other.m_platform), m_device(p_other.m_device), m_globalWorkSize(p_other.m_globalWorkSize), m_localWorkSize(p_other.m_localWorkSize), m_hashesNumber(p_other.m_hashesNumber), m_kernelsPath(p_other.m_kernelsPath) {
}

GeneratorOpenclConfig::~GeneratorOpenclConfig() throw () {
}

GeneratorOpenclConfig& GeneratorOpenclConfig::operator=(const GeneratorOpenclConfig& p_other) {
	if(this == &p_other) {
		return *this;
	}

	m_platform = p_other.m_platform;
	m_device = p_other.m_device;
	m_globalWorkSize = p_other.m_globalWorkSize;
	m_localWorkSize = p_other.m_localWorkSize;
	m_hashesNumber = p_other.m_hashesNumber;
	m_kernelsPath = p_other.m_kernelsPath;

	return *this;
}

void GeneratorOpenclConfig::normalize() {
	if(m_localWorkSize > m_globalWorkSize) {
		m_localWorkSize = m_globalWorkSize;
	}

	if(m_globalWorkSize % m_localWorkSize != 0) {
		m_localWorkSize = std::max((std::size_t)1, m_localWorkSize - m_globalWorkSize % m_localWorkSize);
	}

	if(m_hashesNumber == 0) {
		m_hashesNumber = 1;
	} else if(m_hashesNumber > PLOT_SIZE / HASH_SIZE) {
		m_hashesNumber = PLOT_SIZE / HASH_SIZE;
	}
}

}}}}
