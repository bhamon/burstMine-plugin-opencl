#include <vector>

#include "JsOpenclDevice.h"

namespace cryo {
namespace opencl {
namespace js {

v8::Handle<v8::Value> JsOpenclDevice::New(v8::Isolate* p_isolate, const cryo::opencl::OpenclDevice& p_device) {
	v8::EscapableHandleScope scope(p_isolate);

	std::vector<std::size_t> maxWorkItemSizes(p_device.getMaxWorkItemSizes());
	v8::Local<v8::Array> jsMaxWorkItemSizes = v8::Array::New(p_isolate, static_cast<int>(maxWorkItemSizes.size()));
	for(std::size_t i = 0 ; i < maxWorkItemSizes.size() ; ++i) {
		jsMaxWorkItemSizes->Set(static_cast<int>(i), v8::Integer::New(p_isolate, static_cast<int>(maxWorkItemSizes[i])));
	}

	v8::Local<v8::Object> obj = v8::Object::New(p_isolate);
	obj->Set(v8::String::NewFromUtf8(p_isolate, "type"), v8::String::NewFromUtf8(p_isolate, p_device.getType().c_str()));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "name"), v8::String::NewFromUtf8(p_isolate, p_device.getName().c_str()));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "vendor"), v8::String::NewFromUtf8(p_isolate, p_device.getVendor().c_str()));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "version"), v8::String::NewFromUtf8(p_isolate, p_device.getVersion().c_str()));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "driverVersion"), v8::String::NewFromUtf8(p_isolate, p_device.getDriverVersion().c_str()));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "maxClockFrequency"), v8::Integer::New(p_isolate, p_device.getMaxClockFrequency()));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "maxComputeUnits"), v8::Integer::New(p_isolate, p_device.getMaxComputeUnits()));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "globalMemorySize"), v8::Number::New(p_isolate, static_cast<double>(p_device.getGlobalMemorySize())));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "maxMemoryAllocationSize"), v8::Number::New(p_isolate, static_cast<double>(p_device.getMaxMemoryAllocationSize())));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "maxWorkGroupSize"), v8::Integer::New(p_isolate, static_cast<int>(p_device.getMaxWorkGroupSize())));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "localMemorySize"), v8::Number::New(p_isolate, static_cast<double>(p_device.getLocalMemorySize())));
	obj->Set(v8::String::NewFromUtf8(p_isolate, "maxWorkItemSizes"), jsMaxWorkItemSizes);

	return scope.Escape(obj);
}

}}}