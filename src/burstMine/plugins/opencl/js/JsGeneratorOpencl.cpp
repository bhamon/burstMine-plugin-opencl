/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include <uv.h>
#include <burstMine/js/JsScoopsBuffer.h>
#include <burstMine/js/JsGenerationWork.h>

#include "../../../../opencl/OpenclError.h"
#include "../../../../opencl/js/JsOpenclPlatform.h"
#include "../../../../opencl/js/JsOpenclDevice.h"
#include "../../../../opencl/js/JsOpenclError.h"
#include "JsGeneratorOpencl.h"
#include "JsGeneratorOpenclConfig.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {
namespace js {

v8::Persistent<v8::Function> JsGeneratorOpencl::s_constructor;

void JsGeneratorOpencl::init(v8::Isolate* p_isolate) {
	v8::HandleScope scope(p_isolate);

	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(p_isolate);
	tpl->SetClassName(v8::String::NewFromUtf8(p_isolate, "GeneratorOpencl"));

	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(p_isolate, "config"), JsGeneratorOpencl::getConfig);
	tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(p_isolate, "platform"), JsGeneratorOpencl::getPlatform);
	tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(p_isolate, "device"), JsGeneratorOpencl::getDevice);

	tpl->PrototypeTemplate()->Set(v8::String::NewFromUtf8(p_isolate, "computePlots"), v8::FunctionTemplate::New(p_isolate, JsGeneratorOpencl::computePlots));
	tpl->PrototypeTemplate()->Set(v8::String::NewFromUtf8(p_isolate, "isClosed"), v8::FunctionTemplate::New(p_isolate, JsGeneratorOpencl::isClosed));
	tpl->PrototypeTemplate()->Set(v8::String::NewFromUtf8(p_isolate, "close"), v8::FunctionTemplate::New(p_isolate, JsGeneratorOpencl::close));

	s_constructor.Reset(p_isolate, tpl->GetFunction());
}

v8::Handle<v8::Value> JsGeneratorOpencl::New(v8::Isolate* p_isolate, std::unique_ptr<cryo::burstMine::plugins::opencl::GeneratorOpencl>& p_generator) {
	v8::EscapableHandleScope scope(p_isolate);

	v8::Local<v8::Function> constructor = v8::Local<v8::Function>::New(p_isolate, s_constructor);
	v8::Local<v8::Object> obj = constructor->NewInstance();
	obj->SetInternalField(0, v8::External::New(p_isolate, p_generator.release()));

	return scope.Escape(obj);
}

void JsGeneratorOpencl::unwrap(const v8::PropertyCallbackInfo<v8::Value>& p_info, std::function<void(cryo::burstMine::plugins::opencl::GeneratorOpencl*)> p_functor) {
	v8::HandleScope scope(p_info.GetIsolate());
	v8::Local<v8::Object> self = p_info.Holder();
	v8::Local<v8::External> ext = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	if(ext->Value() == 0) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Generator closed")));
		return;
	}

	p_functor(static_cast<cryo::burstMine::plugins::opencl::GeneratorOpencl*>(ext->Value()));
}

void JsGeneratorOpencl::unwrap(const v8::FunctionCallbackInfo<v8::Value>& p_info, std::function<void(cryo::burstMine::plugins::opencl::GeneratorOpencl*)> p_functor) {
	v8::HandleScope scope(p_info.GetIsolate());
	v8::Local<v8::Object> self = p_info.This();
	v8::Local<v8::External> ext = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	if(ext->Value() == 0) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Generator closed")));
		return;
	}

	p_functor(static_cast<cryo::burstMine::plugins::opencl::GeneratorOpencl*>(ext->Value()));
}

void JsGeneratorOpencl::getConfig(v8::Local<v8::String> p_property, const v8::PropertyCallbackInfo<v8::Value>& p_info) {
	return unwrap(p_info, [&](cryo::burstMine::plugins::opencl::GeneratorOpencl* p_generator) {
		return p_info.GetReturnValue().Set(JsGeneratorOpenclConfig::New(p_info.GetIsolate(), p_generator->getConfig()));
	});
}

void JsGeneratorOpencl::getPlatform(v8::Local<v8::String> p_property, const v8::PropertyCallbackInfo<v8::Value>& p_info) {
	return unwrap(p_info, [&](cryo::burstMine::plugins::opencl::GeneratorOpencl* p_generator) {
		return p_info.GetReturnValue().Set(cryo::opencl::js::JsOpenclPlatform::New(p_info.GetIsolate(), p_generator->getPlatform()));
	});
}

void JsGeneratorOpencl::getDevice(v8::Local<v8::String> p_property, const v8::PropertyCallbackInfo<v8::Value>& p_info) {
	return unwrap(p_info, [&](cryo::burstMine::plugins::opencl::GeneratorOpencl* p_generator) {
		return p_info.GetReturnValue().Set(cryo::opencl::js::JsOpenclDevice::New(p_info.GetIsolate(), p_generator->getDevice()));
	});
}

void JsGeneratorOpencl::computePlots(const v8::FunctionCallbackInfo<v8::Value>& p_info) {
	v8::HandleScope scope(p_info.GetIsolate());

	if(p_info.Length() < 1) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [work] parameter")));
		return;
	} else if(p_info.Length() < 2) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [buffer] parameter")));
		return;
	} else if(p_info.Length() < 3) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [callback] parameter")));
		return;
	} else if(!p_info[0]->IsObject()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [work] parameter")));
		return;
	} else if(!cryo::burstMine::js::JsScoopsBuffer::HasInstance(p_info.GetIsolate(), p_info[1]->ToObject())) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [buffer] parameter")));
		return;
	} else if(!p_info[2]->IsFunction()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [callback] parameter")));
		return;
	}

	std::unique_ptr<cryo::burstMine::plugins::opencl::js::impl::AsyncDataComputePlots> asyncData(new cryo::burstMine::plugins::opencl::js::impl::AsyncDataComputePlots);
	asyncData->isolate = p_info.GetIsolate();
	asyncData->callback.Reset(p_info.GetIsolate(), v8::Local<v8::Function>::Cast(p_info[2]));

	try {
		asyncData->work = std::unique_ptr<cryo::burstMine::GenerationWork>(
			new cryo::burstMine::GenerationWork(cryo::burstMine::js::JsGenerationWork::FromObject(p_info.GetIsolate(), p_info[0]->ToObject()))
		);

		asyncData->buffer = cryo::burstMine::js::JsScoopsBuffer::FromObject(p_info.GetIsolate(), p_info[1]->ToObject());
	} catch(const std::exception& p_ex) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), p_ex.what())));
		return;
	}

	return unwrap(p_info, [&](cryo::burstMine::plugins::opencl::GeneratorOpencl* p_generator) {
		asyncData->generator = p_generator;

		uv_work_t* work = new uv_work_t;
		work->data = static_cast<void*>(asyncData.release());

		uv_queue_work(
			uv_default_loop(),
			work,
			[](uv_work_t* p_work) {
				cryo::burstMine::plugins::opencl::js::impl::AsyncDataComputePlots* asyncData = static_cast<cryo::burstMine::plugins::opencl::js::impl::AsyncDataComputePlots*>(p_work->data);

				try {
					asyncData->generator->computePlots(
						*asyncData->work,
						*asyncData->buffer
					);
				} catch(const std::exception&) {
					asyncData->exception = std::current_exception();
				}
			},
			[](uv_work_t* p_work, int p_status) {
				cryo::burstMine::plugins::opencl::js::impl::AsyncDataComputePlots* asyncData = static_cast<cryo::burstMine::plugins::opencl::js::impl::AsyncDataComputePlots*>(p_work->data);

				asyncData->triggerCallback({});
				delete asyncData;
				delete p_work;
			}
		);
	});
}

void JsGeneratorOpencl::isClosed(const v8::FunctionCallbackInfo<v8::Value>& p_info) {
	v8::EscapableHandleScope scope(p_info.GetIsolate());
	v8::Local<v8::Object> self = p_info.This();

	v8::Local<v8::Boolean> closed = v8::Boolean::New(p_info.GetIsolate(), self->GetInternalField(0)->IsUndefined());
	p_info.GetReturnValue().Set(scope.Escape(closed));
}

void JsGeneratorOpencl::close(const v8::FunctionCallbackInfo<v8::Value>& p_info) {
	v8::HandleScope scope(p_info.GetIsolate());

	if(p_info.Length() < 1) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [callback] parameter")));
		return;
	} else if(!p_info[0]->IsFunction()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [callback] parameter")));
		return;
	}

	std::unique_ptr<cryo::burstMine::plugins::opencl::js::impl::AsyncDataClose> asyncData(new cryo::burstMine::plugins::opencl::js::impl::AsyncDataClose);
	asyncData->isolate = p_info.GetIsolate();
	asyncData->callback.Reset(p_info.GetIsolate(), v8::Local<v8::Function>::Cast(p_info[0]));
	asyncData->self.Reset(p_info.GetIsolate(), p_info.This());

	return unwrap(p_info, [&](cryo::burstMine::plugins::opencl::GeneratorOpencl* p_generator) {
		asyncData->generator = p_generator;

		uv_work_t* work = new uv_work_t;
		work->data = static_cast<void*>(asyncData.release());

		uv_queue_work(
			uv_default_loop(),
			work,
			[](uv_work_t* p_work) {
				cryo::burstMine::plugins::opencl::js::impl::AsyncDataClose* asyncData = static_cast<cryo::burstMine::plugins::opencl::js::impl::AsyncDataClose*>(p_work->data);

				try {
					delete asyncData->generator;

					v8::Local<v8::Object> self = v8::Local<v8::Object>::New(asyncData->isolate, asyncData->self);
					self->SetInternalField(0, v8::Undefined(asyncData->isolate));
				} catch(const std::exception&) {
					asyncData->exception = std::current_exception();
				}
			},
			[](uv_work_t* p_work, int p_status) {
				cryo::burstMine::plugins::opencl::js::impl::AsyncDataClose* asyncData = static_cast<cryo::burstMine::plugins::opencl::js::impl::AsyncDataClose*>(p_work->data);

				asyncData->triggerCallback({});
				delete asyncData;
				delete p_work;
			}
		);
	});
}

}}}}}
