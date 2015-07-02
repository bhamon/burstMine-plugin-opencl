#ifndef CRYO_OPENCL_PLATFORM_H
#define CRYO_OPENCL_PLATFORM_H

#include <string>
#include <vector>
#include <CL/cl.h>

#include "OpenclError.h"

namespace cryo {
namespace opencl {

class OpenclPlatform {
	private:
		cl_platform_id m_handle;

	public:
		OpenclPlatform(const cl_platform_id& p_handle);
		OpenclPlatform(const OpenclPlatform& p_other);
		virtual ~OpenclPlatform() throw ();

		OpenclPlatform& operator=(const OpenclPlatform& p_other);

		inline const cl_platform_id& getHandle() const;

		std::string getName() const throw (OpenclError);
		std::string getVendor() const throw (OpenclError);
		std::string getVersion() const throw (OpenclError);

	private:
		std::string getInfoString(const cl_platform_info& p_paramName) const throw (OpenclError);

	public:
		static std::vector<OpenclPlatform> list() throw (OpenclError);
};

}}

namespace cryo {
namespace opencl {

inline const cl_platform_id& OpenclPlatform::getHandle() const {
	return m_handle;
}

}}

#endif
