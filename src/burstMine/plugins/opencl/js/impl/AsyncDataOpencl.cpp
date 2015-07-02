/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include "../../../../../opencl/OpenclError.h"
#include "../../../../../opencl/js/JsOpenclError.h"
#include "AsyncDataOpencl.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {
namespace js {
namespace impl {

v8::Handle<v8::Value> AsyncDataOpencl::unmarshalException() const {
	v8::Handle<v8::Value> error = v8::Undefined(isolate);

	try {
		if(exception) {
			std::rethrow_exception(exception);
		}
	} catch(const cryo::opencl::OpenclError& p_ex) {
		error = cryo::opencl::js::JsOpenclError::New(isolate, p_ex);
	} catch(const std::exception& p_ex) {
		error = v8::Exception::Error(v8::String::NewFromUtf8(isolate, p_ex.what()));
	}

	return error;
}

}}}}}}
