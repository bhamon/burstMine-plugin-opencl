/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_PLUGINS_OPENCL_JS_JS_GENERATOR_OPENCL_H
#define CRYO_BURST_MINE_PLUGINS_OPENCL_JS_JS_GENERATOR_OPENCL_H

#include <memory>
#include <functional>
#include <exception>
#include <node.h>
#include <node_buffer.h>
#include <burstMine/GenerationWork.h>

#include "../GeneratorOpencl.h"
#include "impl/AsyncDataOpencl.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {
namespace js {

class JsGeneratorOpencl {
	private:
		static v8::Persistent<v8::Function> s_constructor;

	public:
		static void init(v8::Isolate* p_isolate);

		static v8::Handle<v8::Value> New(v8::Isolate* p_isolate, std::unique_ptr<cryo::burstMine::plugins::opencl::GeneratorOpencl>& p_generator);

	private:
		static void unwrap(const v8::PropertyCallbackInfo<v8::Value>& p_info, std::function<void(cryo::burstMine::plugins::opencl::GeneratorOpencl*)> p_functor);
		static void unwrap(const v8::FunctionCallbackInfo<v8::Value>& p_info, std::function<void(cryo::burstMine::plugins::opencl::GeneratorOpencl*)> p_functor);

		static void getConfig(v8::Local<v8::String> p_property, const v8::PropertyCallbackInfo<v8::Value>& p_info);
		static void getPlatform(v8::Local<v8::String> p_property, const v8::PropertyCallbackInfo<v8::Value>& p_info);
		static void getDevice(v8::Local<v8::String> p_property, const v8::PropertyCallbackInfo<v8::Value>& p_info);

		static void open(const v8::FunctionCallbackInfo<v8::Value>& p_info);
		static void computePlots(const v8::FunctionCallbackInfo<v8::Value>& p_info);
		static void isClosed(const v8::FunctionCallbackInfo<v8::Value>& p_info);
		static void close(const v8::FunctionCallbackInfo<v8::Value>& p_info);
};

namespace impl {

class AsyncDataComputePlots : public cryo::burstMine::plugins::opencl::js::impl::AsyncDataOpencl {
	public:
		std::unique_ptr<cryo::burstMine::GenerationWork> work;
		cryo::burstMine::ScoopsBuffer* buffer;
		cryo::burstMine::plugins::opencl::GeneratorOpencl* generator;
};

class AsyncDataClose : public cryo::burstMine::plugins::opencl::js::impl::AsyncDataOpencl {
	public:
		v8::Persistent<v8::Object> self;
		cryo::burstMine::plugins::opencl::GeneratorOpencl* generator;
};

}

}}}}}

#endif
