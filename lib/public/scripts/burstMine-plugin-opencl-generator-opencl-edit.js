/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

context.addController('burstMine-plugin-opencl-generator-opencl-edit', [
	'$scope', '$http', '$modal', 'burstMine-config', 'component',
	function($scope, $http, $modal, p_config, p_component) {
		$scope.error = null;
		$scope.platforms = [];
		$scope.devices = [];

		$scope.data = p_component.params;
		$scope.data.platform = null;
		$scope.data.device = null;
		$scope.data.globalWorkSize = 256;
		$scope.data.localWorkSize = 4;
		$scope.data.hashesNumber = 1;

		$scope.clearError = function() {
			$scope.error = null;
		};

		$scope.loadPlatforms = function() {
			$scope.clearError();
			$scope.platforms = [];
			$scope.data.platform = null;

			$http.get(p_config.apiPath + p_config.plugins.apiPath + '/opencl/platforms')
			.success(function(p_data) {
				$scope.platforms = p_data;
			})
			.error(function(p_data) {
				$scope.error = p_data;
			});
		};

		$scope.loadDevices = function() {
			$scope.clearError();
			$scope.devices = [];
			$scope.data.device = null;

			if(!$scope.data.platform) {
				return;
			}

			$http.get(p_config.apiPath + p_config.plugins.apiPath + '/opencl/platforms/' + $scope.data.platform + '/devices')
			.success(function(p_data) {
				$scope.devices = p_data;
			})
			.error(function(p_data) {
				$scope.clearError(p_data);
			});
		};

		$scope.viewPlatform = function() {
			var modal = $modal.open({
				templateUrl:p_config.publicPath + p_config.plugins.publicPath + '/opencl/templates/burstMine-plugin-opencl-platform-view.html',
				controller:'burstMine-plugin-opencl-platform-view',
				resolve:{
					platform:function() {
						return $scope.platforms[$scope.data.platform];
					}
				}
			});
		};

		$scope.viewDevice = function() {
			var modal = $modal.open({
				templateUrl:p_config.publicPath + p_config.plugins.publicPath + '/opencl/templates/burstMine-plugin-opencl-device-view.html',
				controller:'burstMine-plugin-opencl-device-view',
				resolve:{
					device:function() {
						return $scope.devices[$scope.data.device];
					}
				}
			});
		};

		$scope.loadPlatforms();
	}
]);