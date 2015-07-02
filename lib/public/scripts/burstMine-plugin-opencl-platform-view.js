/*
	Burst mine
	Distributed graphical plotter and miner for Burst.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL
*/

context.addController('burstMine-plugin-opencl-platform-view', [
	'$scope', '$modalInstance', 'platform',
	function($scope, $modalInstance, p_platform) {
		$scope.platform = p_platform;

		$scope.close = function() {
			$modalInstance.close();
		};
	}
]);