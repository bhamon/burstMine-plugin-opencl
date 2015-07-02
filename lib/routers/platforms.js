/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

var lib = {
	deps:{
		q:require('q'),
		joi:require('joi'),
		express:require('express')
	},
	ErrorRest:require('../../../../ErrorRest')
};

var bindings = require('../../bin/burstMine-plugin-opencl');

module.exports.init = function(p_api) {
	var router = lib.deps.express.Router({caseSensitive:true});
	router.get('/', module.exports.list);
	router.get('/:id', module.exports.view);
	router.get('/:id/devices', module.exports.listDevices);
	router.get('/:id/devices/:deviceId', module.exports.viewDevice);

	p_api.use('/platforms', router);
};

module.exports.list = function(p_request, p_response, p_next) {
	lib.deps.q.nfcall(bindings.listPlatforms)
	.then(function(p_platforms) {
		p_response.send(p_platforms);
	})
	.fail(function(p_error) {
		p_next(p_error);
	});
};

module.exports.view = function(p_request, p_response, p_next) {
	p_request.validate({
		params:{
			id:lib.deps.joi.number().integer().min(0)
		}
	});

	lib.deps.q.nfcall(bindings.listPlatforms)
	.then(function(p_platforms) {
		if(p_request.params.id >= p_platforms.length) {
			throw new lib.ErrorRest(404, 'platform.notFound', 'Platform not found');
		}

		p_response.send(p_platforms[p_request.params.id]);
	})
	.fail(function(p_error) {
		p_next(p_error);
	});
};

module.exports.listDevices = function(p_request, p_response, p_next) {
	p_request.validate({
		params:{
			id:lib.deps.joi.number().integer().min(0)
		}
	});

	lib.deps.q.nfcall(bindings.listPlatforms)
	.then(function(p_platforms) {
		if(p_request.params.id >= p_platforms.length) {
			throw new lib.ErrorRest(404, 'platform.notFound', 'Platform not found');
		}
	})
	.then(function() {
		return lib.deps.q.nfcall(bindings.listDevices, p_request.params.id);
	})
	.then(function(p_devices) {
		p_response.send(p_devices);
	})
	.fail(function(p_error) {
		p_next(p_error);
	});
};

module.exports.viewDevice = function(p_request, p_response, p_next) {
	p_request.validate({
		params:{
			id:lib.deps.joi.number().integer().min(0),
			deviceId:lib.deps.joi.number().integer().min(0)
		}
	});

	lib.deps.q.nfcall(bindings.listPlatforms)
	.then(function(p_platforms) {
		if(p_request.params.id >= p_platforms.length) {
			throw new lib.ErrorRest(404, 'platform.notFound', 'Platform not found');
		}

		return p_platforms[p_request.params.id];
	})
	.then(function(p_platform) {
		return lib.deps.q.nfcall(bindings.listDevices, p_platform);
	})
	.then(function(p_devices) {
		if(p_request.params.deviceId >= p_devices.length) {
			throw new lib.ErrorRest(404, 'device.notFound', 'Device not found');
		}

		p_response.send(p_devices[p_request.params.deviceId]);
	})
	.fail(function(p_error) {
		p_next(p_error);
	});
};