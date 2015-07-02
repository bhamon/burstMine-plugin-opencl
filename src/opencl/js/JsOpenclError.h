#ifndef CRYO_OPENCL_JS_JS_OPENCL_ERROR_H
#define CRYO_OPENCL_JS_JS_OPENCL_ERROR_H

#include <node.h>
#include <v8.h>

namespace cryo {
namespace opencl {
namespace js {

class JsOpenclError {
	public:
		static v8::Handle<v8::Value> New(v8::Isolate* p_isolate, const cryo::opencl::OpenclError& p_error);
};

}}}

#endif
