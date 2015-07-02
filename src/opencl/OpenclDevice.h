#ifndef CRYO_OPENCL_DEVICE_H
#define CRYO_OPENCL_DEVICE_H

#include <string>
#include <vector>
#include <CL/cl.h>

#include "OpenclError.h"
#include "OpenclPlatform.h"

namespace cryo {
namespace opencl {

class OpenclDevice {
	private:
		cl_device_id m_handle;

	public:
		OpenclDevice(const cl_device_id& p_handle) throw (OpenclError);
		OpenclDevice(const OpenclDevice& p_other);
		virtual ~OpenclDevice() throw ();

		OpenclDevice& operator=(const OpenclDevice& p_other);

		inline const cl_device_id& getHandle() const;

		std::string getType() const throw (OpenclError);
		std::string getName() const throw (OpenclError);
		std::string getVendor() const throw (OpenclError);
		std::string getVersion() const throw (OpenclError);
		std::string getDriverVersion() const throw (OpenclError);
		unsigned int getMaxClockFrequency() const throw (OpenclError);
		unsigned int getMaxComputeUnits() const throw (OpenclError);
		unsigned long long getGlobalMemorySize() const throw (OpenclError);
		unsigned long long getMaxMemoryAllocationSize() const throw (OpenclError);
		std::size_t getMaxWorkGroupSize() const throw (OpenclError);
		unsigned long long getLocalMemorySize() const throw (OpenclError);
		std::vector<std::size_t> getMaxWorkItemSizes() const throw (OpenclError);

	private:
		std::string getInfoString(const cl_platform_info& p_paramName) const throw (OpenclError);
		unsigned int getInfoUint(const cl_platform_info& p_paramName) const throw (OpenclError);
		std::size_t getInfoSizet(const cl_platform_info& p_paramName) const throw (OpenclError);
		unsigned long long getInfoUlong(const cl_platform_info& p_paramName) const throw (OpenclError);

	public:
		static std::vector<OpenclDevice> list(const OpenclPlatform& p_platform) throw (OpenclError);
};

}}

namespace cryo {
namespace opencl {

inline const cl_device_id& OpenclDevice::getHandle() const {
	return m_handle;
}

}}

#endif
