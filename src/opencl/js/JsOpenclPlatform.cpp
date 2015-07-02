#include "JsOpenclPlatform.h"

namespace cryo {
namespace opencl {
namespace js {

v8::Handle<v8::Value> JsOpenclPlatform::New(v8::Isolate* p_isolate, const cryo::opencl::OpenclPlatform& p_platform) {
	v8::EscapableHandleScope scope(p_isolate);

	v8::Local<v8::Object> obj = v8::Object::New(p_isolate);
	obj->Set(v8::String::NewFromUtf8(p_isolate, "name"), v8::String::NewFromUtf8(p_isolate, p_platform.getName().c_str()));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "vendor"), v8::String::NewFromUtf8(p_isolate, p_platform.getVendor().c_str()));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "version"), v8::String::NewFromUtf8(p_isolate, p_platform.getVersion().c_str()));

	return scope.Escape(obj);
}

}}}