#include "OpenclBuffer.h"

namespace cryo {
namespace opencl {

OpenclBuffer::OpenclBuffer(const cl_mem& p_handle)
: m_handle(p_handle) {
}

OpenclBuffer::~OpenclBuffer() throw () {
	clReleaseMemObject(m_handle);
}

}}
