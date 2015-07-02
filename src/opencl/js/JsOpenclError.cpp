#include "../OpenclError.h"
#include "JsOpenclError.h"

namespace cryo {
namespace opencl {
namespace js {

v8::Handle<v8::Value> JsOpenclError::New(v8::Isolate* p_isolate, const cryo::opencl::OpenclError& p_error) {
	v8::EscapableHandleScope scope(p_isolate);

	v8::Local<v8::Object> error = v8::Exception::Error(v8::String::NewFromUtf8(p_isolate, p_error.what()))->ToObject();
	error->Set(v8::String::NewFromUtf8(p_isolate, "name"), v8::String::NewFromUtf8(p_isolate, "OpenclError"));
	error->Set(v8::String::NewFromUtf8(p_isolate, "code"), v8::Integer::New(p_isolate, p_error.getCode()));
	error->Set(v8::String::NewFromUtf8(p_isolate, "codeString"), v8::String::NewFromUtf8(p_isolate, p_error.getCodeString().c_str()));

	return scope.Escape(error);
}

}}}
