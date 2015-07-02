#include <memory>

#include "OpenclDevice.h"

namespace cryo {
namespace opencl {

OpenclDevice::OpenclDevice(const cl_device_id& p_handle) throw (OpenclError)
: m_handle(p_handle) {
}

OpenclDevice::OpenclDevice(const OpenclDevice& p_other)
: m_handle(p_other.m_handle) {
}

OpenclDevice::~OpenclDevice() throw () {
}

OpenclDevice& OpenclDevice::operator=(const OpenclDevice& p_other) {
	if(this == &p_other) {
		return *this;
	}

	m_handle = p_other.m_handle;

	return *this;
}

std::string OpenclDevice::getType() const throw (OpenclError) {
	cl_device_type type;
	cl_int error = clGetDeviceInfo(m_handle, CL_DEVICE_TYPE, sizeof(cl_device_type), (void*)&type, 0);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve device type");
	}

	switch(type) {
		case CL_DEVICE_TYPE_CPU:
			return "CPU";
		break;
		case CL_DEVICE_TYPE_GPU:
			return "GPU";
		break;
		case CL_DEVICE_TYPE_ACCELERATOR:
			return "Accelerator";
		break;
		default:
			return "Unknown";
	}
}

std::string OpenclDevice::getName() const throw (OpenclError) {
	return getInfoString(CL_DEVICE_NAME);
}

std::string OpenclDevice::getVendor() const throw (OpenclError) {
	return getInfoString(CL_DEVICE_VENDOR);
}

std::string OpenclDevice::getVersion() const throw (OpenclError) {
	return getInfoString(CL_DEVICE_VERSION);
}

std::string OpenclDevice::getDriverVersion() const throw (OpenclError) {
	return getInfoString(CL_DRIVER_VERSION);
}

unsigned int OpenclDevice::getMaxClockFrequency() const throw (OpenclError) {
	return getInfoUint(CL_DEVICE_MAX_CLOCK_FREQUENCY);
}

unsigned int OpenclDevice::getMaxComputeUnits() const throw (OpenclError) {
	return getInfoUint(CL_DEVICE_MAX_COMPUTE_UNITS);
}

unsigned long long OpenclDevice::getGlobalMemorySize() const throw (OpenclError) {
	return getInfoUlong(CL_DEVICE_GLOBAL_MEM_SIZE);
}

unsigned long long OpenclDevice::getMaxMemoryAllocationSize() const throw (OpenclError) {
	return getInfoUlong(CL_DEVICE_MAX_MEM_ALLOC_SIZE);
}

std::size_t OpenclDevice::getMaxWorkGroupSize() const throw (OpenclError) {
	return getInfoSizet(CL_DEVICE_MAX_WORK_GROUP_SIZE);
}

unsigned long long OpenclDevice::getLocalMemorySize() const throw (OpenclError) {
	return getInfoUlong(CL_DEVICE_LOCAL_MEM_SIZE);
}

std::vector<std::size_t> OpenclDevice::getMaxWorkItemSizes() const throw (OpenclError) {
	cl_int error;
	cl_uint maxWorkItemDimensions;
	error = clGetDeviceInfo(m_handle, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS , sizeof(cl_uint), (void*)&maxWorkItemDimensions, 0);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve device max work item dimensions");
	}

	std::unique_ptr<std::size_t[]> maxWorkItemSizes(new std::size_t[maxWorkItemDimensions]);
	error = clGetDeviceInfo(m_handle, CL_DEVICE_MAX_WORK_ITEM_SIZES , sizeof(std::size_t) * maxWorkItemDimensions, (void*)maxWorkItemSizes.get(), 0);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve device max work item sizes");
	}

	return std::vector<std::size_t>(maxWorkItemSizes.get(), maxWorkItemSizes.get() + maxWorkItemDimensions);
}

std::string OpenclDevice::getInfoString(const cl_platform_info& p_paramName) const throw (OpenclError) {
	cl_int error;
	std::size_t size = 0;
	error = clGetDeviceInfo(m_handle, p_paramName, 0, 0, &size);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve device info size");
	}

	std::unique_ptr<char[]> buffer(new char[size]);
	error = clGetDeviceInfo(m_handle, p_paramName, size, (void*)buffer.get(), 0);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve device info value");
	}

	return std::string(buffer.get());
}

unsigned int OpenclDevice::getInfoUint(const cl_platform_info& p_paramName) const throw (OpenclError) {
	cl_uint value = 0;
	cl_int error = clGetDeviceInfo(m_handle, p_paramName , sizeof(cl_uint), (void*)&value, 0);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve device info value");
	}

	return value;
}

std::size_t OpenclDevice::getInfoSizet(const cl_platform_info& p_paramName) const throw (OpenclError) {
	std::size_t value = 0;
	cl_int error = clGetDeviceInfo(m_handle, p_paramName , sizeof(std::size_t), (void*)&value, 0);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve device info value");
	}

	return value;
}

unsigned long long OpenclDevice::getInfoUlong(const cl_platform_info& p_paramName) const throw (OpenclError) {
	cl_ulong value = 0;
	cl_int error = clGetDeviceInfo(m_handle, p_paramName , sizeof(cl_ulong), (void*)&value, 0);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve device info value");
	}

	return value;
}

std::vector<OpenclDevice> OpenclDevice::list(const OpenclPlatform& p_platform) throw (OpenclError) {
	std::vector<OpenclDevice> list;
	cl_int error;

	cl_uint devicesNumber = 0;
	error = clGetDeviceIDs(p_platform.getHandle(), CL_DEVICE_TYPE_ALL, 0, 0, &devicesNumber);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve the OpenCL devices number");
	}

	std::unique_ptr<cl_device_id[]> devices(new cl_device_id[devicesNumber]);
	error = clGetDeviceIDs(p_platform.getHandle(), CL_DEVICE_TYPE_ALL, devicesNumber, devices.get(), 0);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve the OpenCL devices");
	}

	for(cl_uint i = 0 ; i < devicesNumber ; ++i) {
		list.push_back(OpenclDevice(devices[i]));
	}

	return list;
}

}}
