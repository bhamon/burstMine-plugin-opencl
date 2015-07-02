/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_PLUGINS_OPENCL_GENERATOR_OPENCL_CONFIG_H
#define CRYO_BURST_MINE_PLUGINS_OPENCL_GENERATOR_OPENCL_CONFIG_H

#include <string>

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {

class GeneratorOpenclConfig {
	private:
		std::size_t m_platform;
		std::size_t m_device;
		std::size_t m_globalWorkSize;
		std::size_t m_localWorkSize;
		unsigned int m_hashesNumber;
		std::string m_kernelsPath;

	public:
		GeneratorOpenclConfig(std::size_t p_platform, std::size_t p_device, std::size_t p_globalWorkSize, std::size_t p_localWorkSize, unsigned int p_hashesNumber, const std::string p_kernelsPath);
		GeneratorOpenclConfig(const GeneratorOpenclConfig& p_other);
		virtual ~GeneratorOpenclConfig() throw ();

		GeneratorOpenclConfig& operator=(const GeneratorOpenclConfig& p_other);

		inline std::size_t getPlatform() const;
		inline std::size_t getDevice() const;
		inline std::size_t getGlobalWorkSize() const;
		inline std::size_t getLocalWorkSize() const;
		inline unsigned int getHashesNumber() const;
		inline const std::string& getKernelsPath() const;
		inline void setGlobalWorkSize(std::size_t p_globalWorkSize);
		inline void setLocalWorkSize(std::size_t p_localWorkSize);
		inline void setHashesNumber(unsigned int p_hashesNumber);
		inline void setKernelsPath(const std::string& p_kernelPath);
		inline std::size_t& globalWorkSize();
		inline std::size_t& localWorkSize();
		inline unsigned int& hashesNumber();

		inline unsigned long long getBufferSize() const;

		void normalize();
};

}}}}

#include "constants.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {

inline std::size_t GeneratorOpenclConfig::getPlatform() const {
	return m_platform;
}

inline std::size_t GeneratorOpenclConfig::getDevice() const {
	return m_device;
}

inline std::size_t GeneratorOpenclConfig::getGlobalWorkSize() const {
	return m_globalWorkSize;
}

inline std::size_t GeneratorOpenclConfig::getLocalWorkSize() const {
	return m_localWorkSize;
}

inline unsigned int GeneratorOpenclConfig::getHashesNumber() const {
	return m_hashesNumber;
}

inline const std::string& GeneratorOpenclConfig::getKernelsPath() const {
	return m_kernelsPath;
}

inline void GeneratorOpenclConfig::setGlobalWorkSize(std::size_t p_globalWorkSize) {
	m_globalWorkSize = p_globalWorkSize;
}

inline void GeneratorOpenclConfig::setLocalWorkSize(std::size_t p_localWorkSize) {
	m_localWorkSize = p_localWorkSize;
}

inline void GeneratorOpenclConfig::setKernelsPath(const std::string& p_kernelsPath) {
	m_kernelsPath = p_kernelsPath;
}

inline std::size_t& GeneratorOpenclConfig::globalWorkSize() {
	return m_globalWorkSize;
}

inline std::size_t& GeneratorOpenclConfig::localWorkSize() {
	return m_localWorkSize;
}

inline unsigned int& GeneratorOpenclConfig::hashesNumber() {
	return m_hashesNumber;
}

inline void GeneratorOpenclConfig::setHashesNumber(unsigned int p_hashesNumber) {
	m_hashesNumber = p_hashesNumber;
}

inline unsigned long long GeneratorOpenclConfig::getBufferSize() const {
	return m_globalWorkSize * GEN_SIZE;
}

}}}}

#endif
