/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

context.addController('burstMine-plugin-opencl-device-view', [
	'$scope', '$modalInstance', 'device',
	function($scope, $modalInstance, p_device) {
		$scope.device = p_device;

		$scope.close = function() {
			$modalInstance.close();
		};
	}
]);