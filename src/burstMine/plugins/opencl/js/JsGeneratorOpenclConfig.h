/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_PLUGINS_OPENCL_JS_JS_GENERATOR_OPENCL_CONFIG_H
#define CRYO_BURST_MINE_PLUGINS_OPENCL_JS_JS_GENERATOR_OPENCL_CONFIG_H

#include <exception>
#include <node.h>

#include "../GeneratorOpenclConfig.h"

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {
namespace js {

class JsGeneratorOpenclConfig {
	public:
		static v8::Handle<v8::Value> New(v8::Isolate* p_isolate, const cryo::burstMine::plugins::opencl::GeneratorOpenclConfig& p_config);
		static cryo::burstMine::plugins::opencl::GeneratorOpenclConfig FromObject(v8::Isolate* p_isolate, const v8::Handle<v8::Object>& p_object) throw (std::exception);
};

}}}}}

#endif
