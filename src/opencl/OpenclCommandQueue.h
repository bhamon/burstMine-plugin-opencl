#ifndef CRYO_OPENCL_COMMAND_QUEUE_H
#define CRYO_OPENCL_COMMAND_QUEUE_H

#include <vector>
#include <exception>
#include <CL/cl.h>

#include "OpenclKernel.h"
#include "OpenclBuffer.h"
#include "OpenclError.h"

namespace cryo {
namespace opencl {

class OpenclCommandQueue {
	private:
		cl_command_queue m_handle;

	public:
		OpenclCommandQueue(const cl_command_queue& p_handle);
		OpenclCommandQueue(const OpenclCommandQueue& p_other) = delete;
		virtual ~OpenclCommandQueue() throw ();

		OpenclCommandQueue& operator=(const OpenclCommandQueue& p_other) = delete;

		inline const cl_command_queue& getHandle() const;

// TODO: add cl_event support
		void enqueueNDRangeKernel(const OpenclKernel& p_kernel, const std::vector<std::size_t>& p_globalWorkSizes, const std::vector<std::size_t>& p_localWorkSizes) throw (std::exception, OpenclError);
		void enqueueNDRangeKernel(const OpenclKernel& p_kernel, std::size_t p_globalWorkSize, std::size_t p_localWorkSize) throw (std::exception, OpenclError);
		void enqueueReadBuffer(const OpenclBuffer& p_bufferDevice, std::size_t p_offset, std::size_t p_size, unsigned char* p_bufferCpu) throw (OpenclError);
		void flush();
		void finish();
};

}}

namespace cryo {
namespace opencl {

inline const cl_command_queue& OpenclCommandQueue::getHandle() const {
	return m_handle;
}

}}

#endif
