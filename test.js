var lib = {
	node:{
		path:require('path')
	},
	deps:{
		express:require('express')
	}
};

try {
	var bindings = require('build/Release/generator_opencl');
/*
	var platforms = lib.burstMine.generators.opencl.listPlatforms();
	for(var i = 0 ; i < platforms.length ; ++i) {
		console.log(platforms[i]);

		var devices = lib.burstMine.generators.opencl.listDevices(i);
		console.log(devices[i]);
	}
*/
	var context = new bindings.createContext({
		platform:0,
		device:0,
		globalWorkSize:128,
		localWorkSize:2,
		hashesNumber:8192,
		kernelsPath:lib.node.path.resolve(__dirname, 'kernels')
	});

	console.log(context);

	context.computePlots({
		address:123456,
		offset:0,
		workSize:128
	}, function(p_error) {
		if(p_error) {
			console.log(p_error.stack);
		} else {
			console.log('generated!');
		}

		context.close();
	});
} catch(p_ex) {
	console.log(p_ex);
	console.log(p_ex.stack);
}


/*
	lib.q.nfcall(plugins.opencl.listPlatforms)
	.then(function(p_platform) {
		p_platforms.forEach(function(p_platform, p_index) {
			lib.q.nfcall(plugins.opencl.listDevices, p_index)
			.then(function(p_devices) {
				p_devices.forEach(function(p_device) {
					console.log(p_device);
				});
			});
		});
	});
*/
/*
	var generator = plugins.opencl.generators.opencl.create({
		platform:0,
		device:0,
		globalWorkSize:1024,
		localWorkSize:4,
		hashesNumber:8192,
		kernelsPath:'/opt/burstMine/plugins/opencl/kernels'
	});

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
			return lib.q.ninvoke(p_config.generator, 'computePlots', work, buffer);
		})
		.then(function() {
			return lib.q.ninvoke(writer, 'writeNonces', work, buffer);
		});
	};

	var buffer = new Buffer(p_config.generator.maxWorkSize);
	var promise = lib.q.all([
		lib.q.ninvoke(p_config.generator, 'open'),
		lib.q.ninvoke(p_config.writer, 'open')
	]);

	for(var i = 0 ; i < p_config.writer.nonces ; i += p_config.generator.maxWorkSize) {
		promise = step(promise, i);
	}

	promise
	.finally(function() {
		return lib.q.all([
			lib.q.ninvoke(p_config.generator, 'close'),
			lib.q.ninvoke(p_config.writer, 'close')
		]);
	});
};

*/