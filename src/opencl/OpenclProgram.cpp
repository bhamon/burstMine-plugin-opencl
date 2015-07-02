#include "OpenclProgram.h"

namespace cryo {
namespace opencl {

OpenclProgram::OpenclProgram(const cl_program& p_handle)
: m_handle(p_handle) {
}

OpenclProgram::~OpenclProgram() throw () {
	clReleaseProgram(m_handle);
}

std::unique_ptr<OpenclKernel> OpenclProgram::createKernel(const std::string& p_name) const throw (OpenclError) {
	cl_int error;
	cl_kernel kernel = clCreateKernel(m_handle, p_name.c_str(), &error);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to create the OpenCL kernel");
	}

	return std::unique_ptr<OpenclKernel>(new OpenclKernel(kernel));
}

}}
