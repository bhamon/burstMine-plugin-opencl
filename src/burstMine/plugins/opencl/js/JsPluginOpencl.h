/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_PLUGINS_OPENCL_JS_JS_PLUGIN_OPENCL_H
#define CRYO_BURST_MINE_PLUGINS_OPENCL_JS_JS_PLUGIN_OPENCL_H

#include <memory>
#include <vector>
#include <node.h>

#include "../../../../opencl/OpenclPlatform.h"
#include "../../../../opencl/OpenclDevice.h"
#include "../GeneratorOpenclConfig.h"
#include "impl/AsyncDataOpencl.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {
namespace js {

class JsPluginOpencl {
	public:
		static void init(v8::Handle<v8::Object> p_exports);

	private:
		static void createGenerator(const v8::FunctionCallbackInfo<v8::Value>& p_info);
		static void listPlatforms(const v8::FunctionCallbackInfo<v8::Value>& p_info);
		static void listDevices(const v8::FunctionCallbackInfo<v8::Value>& p_info);
};

namespace impl {

class AsyncDataCreateGenerator : public AsyncDataOpencl {
	public:
		std::unique_ptr<cryo::burstMine::plugins::opencl::GeneratorOpenclConfig> config;
		std::unique_ptr<cryo::burstMine::plugins::opencl::GeneratorOpencl> generator;
};

class AsyncDataListPlatforms : public AsyncDataOpencl {
	public:
		std::vector<cryo::opencl::OpenclPlatform> platforms;
};

class AsyncDataListDevices : public AsyncDataOpencl {
	public:
		std::size_t platformId;
		std::vector<cryo::opencl::OpenclDevice> devices;
};

}

}}}}}

#endif
