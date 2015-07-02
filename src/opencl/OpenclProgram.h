#ifndef CRYO_OPENCL_PROGRAM_H
#define CRYO_OPENCL_PROGRAM_H

#include <memory>
#include <CL/cl.h>

#include "OpenclKernel.h"
#include "OpenclError.h"

namespace cryo {
namespace opencl {

class OpenclProgram {
	private:
		cl_program m_handle;

	public:
		OpenclProgram(const cl_program& p_handle);
		OpenclProgram(const OpenclProgram& p_other) = delete;
		virtual ~OpenclProgram() throw ();

		OpenclProgram& operator=(const OpenclProgram& p_other) = delete;

		inline const cl_program& getHandle() const;

		std::unique_ptr<OpenclKernel> createKernel(const std::string& p_name) const throw (OpenclError);
};

}}

namespace cryo {
namespace opencl {

inline const cl_program& OpenclProgram::getHandle() const {
	return m_handle;
}

}}

#endif
