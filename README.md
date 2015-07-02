# OpenCL plugin for Burst Mine.

This is a plugin for [Burst Mine](https://github.com/bhamon/burstMine).

- Author: Cryo
- Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
- Burst: BURST-YA29-QCEW-QXC3-BKXDL

This plugin takes advantage of your graphic card computing capabilities by using the standard OpenCL library to enhance Burst operations (plots generation, deadline check).

If you like this software, support me.

## Build instructions

Please refer to the [INSTALL](INSTALL.md) guide.

## Troubleshooting

#### Error while loading the OpenCL plugin.

Make sure your OpenCL dynamic library is in your path.
If it's not the case, you have two options :

- Add the OpenCL dynamic libraries directory to your path.
- Copy/paste the OpenCL dynamic library to the [bin] folder of this plugin (besides the [burstMine-plugin-opencl.node] file).

If your driver crashes (screen goes dark, ...) when using this plugin, please make sure to reduce the [intensity] factor.

#### No platform is detected on the computer.
#### No device is found on the platforms.

Make sure your hardware is OpenCL compliant. Make sure you have the last drivers installed.

#### After launch, the display freezes and the screen goes black. Few seconds later, the display comes back but the program fails with an OpenCL error and the OS display an information message about a display driver crash.
#### After launch, the display freezes, the screen goes black and, finally, the computer reboots.

The graphic card you use is bound to your display. As the plot generation is an heavy process, it uses all of your graphic card resources in one call. To prevent display crashes, the driver kills all kernels that hang for too long.

To solve this issue, the global work needs to be split. This is the purpose of the [hashesNumber] parameter. Begin with a low value (from 1 to 10), and try to increase it until you experience some display lags.

#### The nonces/minutes are incredibly low (no more than few hundreds).

There are many causes for that:

- The configured [globalWorkSize] and [localWorkSize] for your cards are not optimal. Try with different values.
- Some of your cards causes bottlenecks. Tests them one after another to detect the one causing it and try to improve its parameters.
- For NVidia owners: OpenCL implementation is under-efficient on some NVidia cards. Try to tweak your device parameters. If there is no significant improvements, you'll need a CUDA version of this program.