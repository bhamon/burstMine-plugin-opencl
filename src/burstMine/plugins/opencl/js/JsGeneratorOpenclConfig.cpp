/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include <stdexcept>

#include "JsGeneratorOpenclConfig.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {
namespace js {

v8::Handle<v8::Value> JsGeneratorOpenclConfig::New(v8::Isolate* p_isolate, const cryo::burstMine::plugins::opencl::GeneratorOpenclConfig& p_config) {
	v8::EscapableHandleScope scope(p_isolate);

	v8::Local<v8::Object> obj = v8::Object::New(p_isolate);
	obj->Set(v8::String::NewFromUtf8(p_isolate, "platform"), v8::Integer::New(p_isolate, static_cast<int>(p_config.getPlatform())));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "device"), v8::Integer::New(p_isolate, static_cast<int>(p_config.getDevice())));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "globalWorkSize"), v8::Integer::New(p_isolate, static_cast<int>(p_config.getGlobalWorkSize())));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "localWorkSize"), v8::Integer::New(p_isolate, static_cast<int>(p_config.getLocalWorkSize())));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "hashesNumber"), v8::Integer::New(p_isolate, p_config.getHashesNumber()));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "kernelsPath"), v8::String::NewFromUtf8(p_isolate, p_config.getKernelsPath().c_str()));

	return scope.Escape(obj);
}

cryo::burstMine::plugins::opencl::GeneratorOpenclConfig JsGeneratorOpenclConfig::FromObject(v8::Isolate* p_isolate, const v8::Handle<v8::Object>& p_object) throw (std::exception) {
	v8::HandleScope scope(p_isolate);

	if(!p_object->Has(v8::String::NewFromUtf8(p_isolate, "platform"))) {
		throw std::runtime_error("Missing [platform] parameter");
	} else if(!p_object->Has(v8::String::NewFromUtf8(p_isolate, "device"))) {
		throw std::runtime_error("Missing [device] parameter");
	} else if(!p_object->Has(v8::String::NewFromUtf8(p_isolate, "globalWorkSize"))) {
		throw std::runtime_error("Missing [globalWorkSize] parameter");
	} else if(!p_object->Has(v8::String::NewFromUtf8(p_isolate, "localWorkSize"))) {
		throw std::runtime_error("Missing [localWorkSize] parameter");
	} else if(!p_object->Has(v8::String::NewFromUtf8(p_isolate, "hashesNumber"))) {
		throw std::runtime_error("Missing [hashesNumber] parameter");
	} else if(!p_object->Has(v8::String::NewFromUtf8(p_isolate, "kernelsPath"))) {
		throw std::runtime_error("Missing [kernelsPath] parameter");
	} else if(!p_object->Get(v8::String::NewFromUtf8(p_isolate, "platform"))->IsNumber()) {
		throw std::runtime_error("Invalid [platform] parameter");
	} else if(!p_object->Get(v8::String::NewFromUtf8(p_isolate, "device"))->IsNumber()) {
		throw std::runtime_error("Invalid [device] parameter");
	} else if(!p_object->Get(v8::String::NewFromUtf8(p_isolate, "globalWorkSize"))->IsNumber()) {
		throw std::runtime_error("Invalid [globalWorkSize] parameter");
	} else if(!p_object->Get(v8::String::NewFromUtf8(p_isolate, "localWorkSize"))->IsNumber()) {
		throw std::runtime_error("Invalid [localWorkSize] parameter");
	} else if(!p_object->Get(v8::String::NewFromUtf8(p_isolate, "hashesNumber"))->IsNumber()) {
		throw std::runtime_error("Invalid [hashesNumber] parameter");
	} else if(!p_object->Get(v8::String::NewFromUtf8(p_isolate, "kernelsPath"))->IsString()) {
		throw std::runtime_error("Invalid [kernelsPath] parameter");
	}

	v8::String::Utf8Value kernelsPath(p_object->Get(v8::String::NewFromUtf8(p_isolate, "kernelsPath")));
	return cryo::burstMine::plugins::opencl::GeneratorOpenclConfig(
		p_object->Get(v8::String::NewFromUtf8(p_isolate, "platform"))->Uint32Value(),
		p_object->Get(v8::String::NewFromUtf8(p_isolate, "device"))->Uint32Value(),
		p_object->Get(v8::String::NewFromUtf8(p_isolate, "globalWorkSize"))->Uint32Value(),
		p_object->Get(v8::String::NewFromUtf8(p_isolate, "localWorkSize"))->Uint32Value(),
		p_object->Get(v8::String::NewFromUtf8(p_isolate, "hashesNumber"))->Uint32Value(),
		*kernelsPath
	);
}

}}}}}
