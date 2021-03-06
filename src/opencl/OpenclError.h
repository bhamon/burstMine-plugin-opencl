#ifndef CRYO_OPENCL_ERROR_H
#define CRYO_OPENCL_ERROR_H

#include <string>
#include <stdexcept>
#include <CL/cl.h>

namespace cryo {
namespace opencl {

class OpenclError : public std::runtime_error {
	private:
		cl_int m_code;

	public:
		OpenclError(cl_int p_code, const std::string& p_message);
		OpenclError(cl_int p_code, const char* p_message);
		OpenclError(const OpenclError& p_other);
		virtual ~OpenclError() throw ();

		OpenclError& operator=(const OpenclError& p_other);

		inline cl_int getCode() const;
		std::string getCodeString() const;
};

}}

namespace cryo {
namespace opencl {

inline cl_int OpenclError::getCode() const {
	return m_code;
}

}}

#endif
