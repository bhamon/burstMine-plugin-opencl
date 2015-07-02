#ifndef CRYO_OPENCL_JS_JS_OPENCL_DEVICE_H
#define CRYO_OPENCL_JS_JS_OPENCL_DEVICE_H

#include <node.h>
#include <v8.h>

#include "../OpenclDevice.h"

namespace cryo {
namespace opencl {
namespace js {

class JsOpenclDevice {
	public:
		static v8::Handle<v8::Value> New(v8::Isolate* p_isolate, const cryo::opencl::OpenclDevice& p_device);
};

}}}

#endif
