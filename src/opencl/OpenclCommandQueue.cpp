#include <memory>
#include <stdexcept>

#include "OpenclCommandQueue.h"

namespace cryo {
namespace opencl {

OpenclCommandQueue::OpenclCommandQueue(const cl_command_queue& p_handle)
: m_handle(p_handle) {
}

OpenclCommandQueue::~OpenclCommandQueue() throw () {
	clReleaseCommandQueue(m_handle);
}

void OpenclCommandQueue::enqueueNDRangeKernel(const OpenclKernel& p_kernel, const std::vector<std::size_t>& p_globalWorkSizes, const std::vector<std::size_t>& p_localWorkSizes) throw (std::exception, OpenclError) {
	if(p_globalWorkSizes.size() != p_localWorkSizes.size()) {
		throw std::runtime_error("Global and local work sizes arrays differ in length");
	}

	std::size_t workDimensions = p_globalWorkSizes.size();
	if(workDimensions == 0 || workDimensions > 3) {
		throw std::runtime_error("Invalid work dimensions");
	}

	std::unique_ptr<std::size_t[]> globalWorkSizes(new std::size_t[workDimensions]);
	std::unique_ptr<std::size_t[]> localWorkSizes(new std::size_t[workDimensions]);
	for(std::size_t i = 0 ; i < workDimensions ; ++i) {
		globalWorkSizes[i] = p_globalWorkSizes[i];
		localWorkSizes[i] = p_localWorkSizes[i];
	}

	cl_int error = clEnqueueNDRangeKernel(m_handle, p_kernel.getHandle(), static_cast<cl_uint>(workDimensions), 0, globalWorkSizes.get(), localWorkSizes.get(), 0, 0, 0);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Error while enqueuing kernel");
	}
}

void OpenclCommandQueue::enqueueNDRangeKernel(const OpenclKernel& p_kernel, std::size_t p_globalWorkSize, std::size_t p_localWorkSize) throw (std::exception, OpenclError) {
	std::vector<std::size_t> globalWorkSizes;
	globalWorkSizes.push_back(p_globalWorkSize);

	std::vector<std::size_t> localWorkSizes;
	localWorkSizes.push_back(p_localWorkSize);

	enqueueNDRangeKernel(p_kernel, globalWorkSizes, localWorkSizes);
}

void OpenclCommandQueue::enqueueReadBuffer(const OpenclBuffer& p_bufferDevice, std::size_t p_offset, std::size_t p_size, unsigned char* p_bufferCpu) throw (OpenclError) {
	cl_int error = clEnqueueReadBuffer(m_handle, p_bufferDevice.getHandle(), CL_TRUE, p_offset, p_size, p_bufferCpu, 0, 0, 0);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Error in synchronous read");
	}
}

void OpenclCommandQueue::flush() {
	cl_int error = clFlush(m_handle);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Error while flushing queued jobs");
	}
}

void OpenclCommandQueue::finish() {
	cl_int error = clFinish(m_handle);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Error while finishing queued jobs");
	}
}

}}
