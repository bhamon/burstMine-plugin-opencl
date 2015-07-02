#ifndef CRYO_OPENCL_JS_JS_OPENCL_PLATFORM_H
#define CRYO_OPENCL_JS_JS_OPENCL_PLATFORM_H

#include <node.h>
#include <v8.h>

#include "../OpenclPlatform.h"

namespace cryo {
namespace opencl {
namespace js {

class JsOpenclPlatform {
	public:
		static v8::Handle<v8::Value> New(v8::Isolate* p_isolate, const cryo::opencl::OpenclPlatform& p_platform);
};

}}}

#endif
