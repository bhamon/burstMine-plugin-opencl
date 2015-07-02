/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

var lib = {
	node:{
		path:require('path')
	},
	deps:{
		q:require('q'),
		joi:require('joi')
	},
	ErrorRest:require('../../../../ErrorRest')
};

var bindings = require('../../bin/burstMine-plugin-opencl');

var GeneratorOpencl = function GeneratorOpencl(p_native) {
	Object.defineProperty(this, '_native', {value:p_native});
	Object.defineProperty(this, 'platform', {enumerable:true, value:p_native.platform});
	Object.defineProperty(this, 'device', {enumerable:true, value:p_native.device});
	Object.defineProperty(this, 'config', {enumerable:true, value:p_native.config});
	Object.defineProperty(this, 'maxWorkSize', {enumerable:true, value:p_native.config.globalWorkSize});
};

GeneratorOpencl.prototype.computePlots = function(p_work, p_buffer) {
	return lib.deps.q.ninvoke(this._native, 'computePlots', p_work, p_buffer);
};

GeneratorOpencl.prototype.close = function() {
	return lib.deps.q.ninvoke(this._native, 'close');
};

module.exports.parseParams = function(p_generatorPreset, p_params) {
	var result = lib.deps.joi.validate(p_params, {
		platform:lib.deps.joi.number().integer().min(0),
		device:lib.deps.joi.number().integer().min(0),
		globalWorkSize:lib.deps.joi.number().integer().min(1),
		localWorkSize:lib.deps.joi.number().integer().min(1),
		hashesNumber:lib.deps.joi.number().integer().min(1).max(8192)
	}, {
		abortEarly:false,
		presence:'required'
	});

	if(result.error) {
		throw new lib.ErrorRest(409, 'validation', 'Invalid input parameters', result.error.details);
	}

	var postResult = lib.deps.joi.validate(result.value, {
		globalWorkSize:lib.deps.joi.number().multiple(result.value.localWorkSize),
		localWorkSize:lib.deps.joi.number().max(result.value.globalWorkSize)
	}, {
		abortEarly:false,
		allowUnknown:true,
		presence:'required'
	});

	if(postResult.error) {
		throw new lib.ErrorRest(409, 'validation', 'Invalid input parameters', postResult.error.details);
	}

	result.value.kernelsPath = lib.node.path.resolve(__dirname, '..', 'kernels');

	return result.value;
};

module.exports.create = function(p_context, p_generatorPreset) {
	return lib.deps.q.nfcall(bindings.createGenerator, p_generatorPreset.params)
	.then(function(p_generator) {
		return new GeneratorOpencl(p_generator);
	});
};