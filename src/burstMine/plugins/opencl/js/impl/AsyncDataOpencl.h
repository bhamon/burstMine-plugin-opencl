/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

#ifndef CRYO_BURST_MINE_PLUGINS_OPENCL_JS_IMPL_ASYNC_DATA_OPENCL_H
#define CRYO_BURST_MINE_PLUGINS_OPENCL_JS_IMPL_ASYNC_DATA_OPENCL_H

#include <burstMine/js/impl/AsyncData.h>

namespace cryo {
namespace burstMine {
namespace plugins {
namespace opencl {
namespace js {
namespace impl {

class AsyncDataOpencl : public cryo::burstMine::js::impl::AsyncData {
	protected:
		virtual v8::Handle<v8::Value> unmarshalException() const;
};

}}}}}}

#endif
