/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#include <functional>
#include <uv.h>

#include "../../../../opencl/OpenclError.h"
#include "../../../../opencl/js/JsOpenclPlatform.h"
#include "../../../../opencl/js/JsOpenclDevice.h"
#include "../../../../opencl/js/JsOpenclError.h"
#include "../GeneratorOpencl.h"
#include "JsPluginOpencl.h"
#include "JsGeneratorOpencl.h"
#include "JsGeneratorOpenclConfig.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {
namespace js {

void JsPluginOpencl::init(v8::Handle<v8::Object> p_exports) {
	JsGeneratorOpencl::init(v8::Isolate::GetCurrent());

	p_exports->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "createGenerator"), v8::FunctionTemplate::New(v8::Isolate::GetCurrent(), createGenerator)->GetFunction());
	p_exports->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "listPlatforms"), v8::FunctionTemplate::New(v8::Isolate::GetCurrent(), listPlatforms)->GetFunction());
	p_exports->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "listDevices"), v8::FunctionTemplate::New(v8::Isolate::GetCurrent(), listDevices)->GetFunction());
}

void JsPluginOpencl::createGenerator(const v8::FunctionCallbackInfo<v8::Value>& p_info) {
	v8::HandleScope scope(p_info.GetIsolate());

	if(p_info.Length() < 1) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [config] parameter")));
		return;
	} else if(p_info.Length() < 2) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [callback] parameter")));
		return;
	} else if(!p_info[0]->IsObject()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [config] parameter")));
		return;
	} else if(!p_info[1]->IsFunction()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [callback] parameter")));
		return;
	}

	std::unique_ptr<cryo::burstMine::plugins::opencl::js::impl::AsyncDataCreateGenerator> asyncData(new cryo::burstMine::plugins::opencl::js::impl::AsyncDataCreateGenerator);
	asyncData->isolate = p_info.GetIsolate();
	asyncData->callback.Reset(p_info.GetIsolate(), v8::Local<v8::Function>::Cast(p_info[1]));

	try {
		asyncData->config = std::unique_ptr<cryo::burstMine::plugins::opencl::GeneratorOpenclConfig>(
			new cryo::burstMine::plugins::opencl::GeneratorOpenclConfig(
				JsGeneratorOpenclConfig::FromObject(p_info.GetIsolate(), v8::Local<v8::Object>::Cast(p_info[0]))
			)
		);
	} catch(const std::exception& p_ex) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), p_ex.what())));
		return;
	}

	uv_work_t* work = new uv_work_t;
	work->data = static_cast<void*>(asyncData.release());

	uv_queue_work(
		uv_default_loop(),
		work,
		[](uv_work_t* p_work) {
			cryo::burstMine::plugins::opencl::js::impl::AsyncDataCreateGenerator* asyncData = static_cast<cryo::burstMine::plugins::opencl::js::impl::AsyncDataCreateGenerator*>(p_work->data);

			try {
				asyncData->generator = std::unique_ptr<cryo::burstMine::plugins::opencl::GeneratorOpencl>(
					new cryo::burstMine::plugins::opencl::GeneratorOpencl(*asyncData->config)
				);
			} catch(const std::exception&) {
				asyncData->exception = std::current_exception();
			}
		},
		[](uv_work_t* p_work, int p_status) {
			cryo::burstMine::plugins::opencl::js::impl::AsyncDataCreateGenerator* asyncData = static_cast<cryo::burstMine::plugins::opencl::js::impl::AsyncDataCreateGenerator*>(p_work->data);

			v8::Handle<v8::Value> jsGenerator = JsGeneratorOpencl::New(asyncData->isolate, asyncData->generator);

			asyncData->triggerCallback({jsGenerator});
			delete asyncData;
			delete p_work;
		}
	);
}

void JsPluginOpencl::listPlatforms(const v8::FunctionCallbackInfo<v8::Value>& p_info) {
	v8::HandleScope scope(p_info.GetIsolate());

	if(p_info.Length() < 1) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [callback] parameter")));
		return;
	} else if(!p_info[0]->IsFunction()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [callback] parameter")));
		return;
	}

	std::unique_ptr<cryo::burstMine::plugins::opencl::js::impl::AsyncDataListPlatforms> asyncData(new cryo::burstMine::plugins::opencl::js::impl::AsyncDataListPlatforms);
	asyncData->isolate = p_info.GetIsolate();
	asyncData->callback.Reset(p_info.GetIsolate(), v8::Local<v8::Function>::Cast(p_info[0]));

	uv_work_t* work = new uv_work_t;
	work->data = static_cast<void*>(asyncData.release());

	uv_queue_work(
		uv_default_loop(),
		work,
		[](uv_work_t* p_work) {
			cryo::burstMine::plugins::opencl::js::impl::AsyncDataListPlatforms* asyncData = static_cast<cryo::burstMine::plugins::opencl::js::impl::AsyncDataListPlatforms*>(p_work->data);

			try {
				asyncData->platforms = cryo::opencl::OpenclPlatform::list();
			} catch(const std::exception&) {
				asyncData->exception = std::current_exception();
			}
		},
		[](uv_work_t* p_work, int p_status) {
			cryo::burstMine::plugins::opencl::js::impl::AsyncDataListPlatforms* asyncData = static_cast<cryo::burstMine::plugins::opencl::js::impl::AsyncDataListPlatforms*>(p_work->data);

			v8::Local<v8::Array> jsPlatforms = v8::Array::New(asyncData->isolate, static_cast<int>(asyncData->platforms.size()));
			for(std::size_t i = 0 ; i < asyncData->platforms.size() ; ++i) {
				jsPlatforms->Set(static_cast<int>(i), cryo::opencl::js::JsOpenclPlatform::New(asyncData->isolate, asyncData->platforms[i]));
			}

			asyncData->triggerCallback({jsPlatforms});
			delete asyncData;
			delete p_work;
		}
	);
}

void JsPluginOpencl::listDevices(const v8::FunctionCallbackInfo<v8::Value>& p_info) {
	v8::HandleScope scope(p_info.GetIsolate());

	if(p_info.Length() < 1) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [platform] parameter")));
		return;
	} else if(p_info.Length() < 1) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Missing [callback] parameter")));
		return;
	} else if(!p_info[0]->IsNumber()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [platform] parameter")));
		return;
	} else if(!p_info[1]->IsFunction()) {
		p_info.GetIsolate()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(p_info.GetIsolate(), "Invalid [callback] parameter")));
		return;
	}

	std::unique_ptr<cryo::burstMine::plugins::opencl::js::impl::AsyncDataListDevices> asyncData(new cryo::burstMine::plugins::opencl::js::impl::AsyncDataListDevices);
	asyncData->isolate = p_info.GetIsolate();
	asyncData->callback.Reset(p_info.GetIsolate(), v8::Local<v8::Function>::Cast(p_info[1]));
	asyncData->platformId = p_info[0]->Uint32Value();

	uv_work_t* work = new uv_work_t;
	work->data = static_cast<void*>(asyncData.release());

	uv_queue_work(
		uv_default_loop(),
		work,
		[](uv_work_t* p_work) {
			cryo::burstMine::plugins::opencl::js::impl::AsyncDataListDevices* asyncData = static_cast<cryo::burstMine::plugins::opencl::js::impl::AsyncDataListDevices*>(p_work->data);

			try {
				std::vector<cryo::opencl::OpenclPlatform> platforms(cryo::opencl::OpenclPlatform::list());
				if(asyncData->platformId >= platforms.size()) {
					throw std::runtime_error("Unknown [platform]");
				}

				asyncData->devices = cryo::opencl::OpenclDevice::list(platforms[asyncData->platformId]);
			} catch(const std::exception&) {
				asyncData->exception = std::current_exception();
			}
		},
		[](uv_work_t* p_work, int p_status) {
			cryo::burstMine::plugins::opencl::js::impl::AsyncDataListDevices* asyncData = static_cast<cryo::burstMine::plugins::opencl::js::impl::AsyncDataListDevices*>(p_work->data);

			v8::Local<v8::Array> jsDevices = v8::Array::New(asyncData->isolate, static_cast<int>(asyncData->devices.size()));
			for(std::size_t i = 0 ; i < asyncData->devices.size() ; ++i) {
				jsDevices->Set(static_cast<int>(i), cryo::opencl::js::JsOpenclDevice::New(asyncData->isolate, asyncData->devices[i]));
			}

			asyncData->triggerCallback({jsDevices});
			delete asyncData;
			delete p_work;
		}
	);
}

}}}}}

NODE_MODULE(burstMine_plugin_opencl, cryo::burstMine::plugins::opencl::js::JsPluginOpencl::init);
