var lib = {
	node:{
		path:require('path')
	},
	deps:{
		q:require('q')
	}
};

var bindings = require('./bin/burstMine-plugin-opencl');
/*
lib.deps.q.nfcall(bindings.listPlatforms)
.then(function(p_platforms) {
	p_platforms.forEach(function(p_platform, p_index) {
		lib.deps.q.nfcall(bindings.listDevices, p_index)
		.then(function(p_devices) {
			p_devices.forEach(function(p_device) {
				console.log(p_device);
			});
		});
	});
});
*/
lib.deps.q.nfcall(bindings.createGenerator, {
	platform:0,
	device:0,
	globalWorkSize:128,
	localWorkSize:4,
	hashesNumber:8192,
	kernelsPath:lib.node.path.resolve(__dirname, 'lib', 'kernels')
})
.then(function(p_generator) {
	console.log('Generator created');
	var buffer = new Buffer(4096 * 64 * p_generator.config.globalWorkSize);

	return lib.deps.q.ninvoke(p_generator, 'computePlots', {
		address:123456,
		offset:0,
		size:p_generator.config.globalWorkSize
	}, buffer)
	.then(function() {
		console.log('Plots generated');
		lib.deps.q.ninvoke(p_generator, 'close');
	});
})
.then(function() {
	console.log('Generator closed');
})
.done();

/*
	var writer = plugins.js.writers.fileStaggered.create({
		path:p_config.path,
		address:p_config.address,
		offset:p_config.offset,
		nonces:p_config.nonces,
		preAllocate:p_config.preAllocate,
		staggerSize:p_config.staggerSize
	});

	// generation
	plugins.js.strategies.singleStreamed.generate({
		generator:generator,
		writer:writer
	})
	.then(function() {
		console.log('ok!');
	});

//---------------------------------

plugins.js.strategies.singleStreamed.generate = function(p_config) {
	var step = function(p_promise, p_nb) {
		var work = {
			address:p_config.writer.address,
			offset:p_config.writer.offset + p_nb,
			size:Math.min(p_config.generator.maxWorkSize, p_config.writer.nonces - p_nb)
		};

		return p_promise
		.then(function() {
			return lib.deps.q.ninvoke(p_config.generator, 'computePlots', work, buffer);
		})
		.then(function() {
			return lib.deps.q.ninvoke(writer, 'writeNonces', work, buffer);
		});
	};

	var buffer = new Buffer(p_config.generator.maxWorkSize);
	var promise = lib.deps.q.all([
		lib.deps.q.ninvoke(p_config.generator, 'open'),
		lib.deps.q.ninvoke(p_config.writer, 'open')
	]);

	for(var i = 0 ; i < p_config.writer.nonces ; i += p_config.generator.maxWorkSize) {
		promise = step(promise, i);
	}

	promise
	.finally(function() {
		return lib.deps.q.all([
			lib.deps.q.ninvoke(p_config.generator, 'close'),
			lib.deps.q.ninvoke(p_config.writer, 'close')
		]);
	});
};
*/