var lib = {
	node:{
		path:require('path')
	},
	deps:{
		q:require('q')
	}
};

var bindings = {
	core:require('../../../bin/burstMine'),
	opencl:require('./bin/burstMine-plugin-opencl')
};

lib.deps.q.nfcall(bindings.opencl.createGenerator, {
	platform:0,
	device:0,
	globalWorkSize:1024,
	localWorkSize:4,
	hashesNumber:1,
	kernelsPath:lib.node.path.resolve(__dirname, 'lib', 'kernels')
})
.then(function(p_generator) {
	console.log('Generator created');
	var buffer = bindings.core.createScoopsBuffer(p_generator.config.globalWorkSize * 4096 * 4);

	return lib.deps.q.ninvoke(p_generator, 'computePlots', {
		address:123456,
		offset:0,
		size:p_generator.config.globalWorkSize
	}, buffer)
	.then(function() {
		console.log('Plots generated');
		lib.deps.q.ninvoke(p_generator, 'close');
	})
	.finally(function() {
		buffer.close();
	});
})
.then(function() {
	console.log('Generator closed');
})
.done();