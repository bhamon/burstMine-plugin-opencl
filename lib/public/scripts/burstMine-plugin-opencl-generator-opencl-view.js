/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

context.addController('burstMine-plugin-opencl-generator-opencl-view', [
	'$scope', '$q', '$http', '$modal', 'burstMine-config', 'component',
	function($scope, $q, $http, $modal, p_config, p_component) {
		$scope.error = null;
		$scope.generator = p_component;
		$scope.platform = null;
		$scope.device = null;

		$scope.clearError = function() {
			$scope.error = null;
		};

		$scope.loadPlatform = function() {
			var defer = $q.defer();
			$scope.clearError();

			$http.get(p_config.apiPath + p_config.plugins.apiPath + '/opencl/platforms/' + $scope.generator.platform)
			.success(function(p_data) {
				$scope.platform = p_data;
				defer.resolve();
			})
			.error(function(p_data) {
				$scope.clearError(p_data);
				$scope.reject(p_data);
			});

			return defer.promise;
		};

		$scope.loadDevice = function() {
			var defer = $q.defer();
			$scope.clearError();

			$http.get(p_config.apiPath + p_config.plugins.apiPath + '/opencl/platforms/' + $scope.generator.platform + '/devices/' + $scope.generator.device)
			.success(function(p_data) {
				$scope.device = p_data;
				defer.resolve();
			})
			.error(function(p_data) {
				$scope.clearError(p_data);
				$scope.reject(p_data);
			});

			return defer.promise;
		};

		$scope.viewPlatform = function() {
			var modal = $modal.open({
				templateUrl:p_config.publicPath + p_config.plugins.publicPath + '/opencl/templates/burstMine-plugin-opencl-platform-view.html',
				controller:'burstMine-plugin-opencl-platform-view',
				resolve:{
					platform:function() {
						return $scope.platform;
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
						return $scope.device;
					}
				}
			});
		};

		$q.all([
			$scope.loadPlatform(),
			$scope.loadDevice()
		]);
	}
]);