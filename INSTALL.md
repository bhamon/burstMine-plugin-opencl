# Installation guide

## Requirements

This plugin needs to be located in the [lib/plugins] folder of your Burst Mine installation.

To build this plugin, you need a functional installation of NodeJS + NPM + compiler (GCC / MSVC / ...).

This plugin requires a valid OpenCL installation.
This is usually achieved by downloading and installing the last driver or SDK for your graphic card.
Please refer to your graphic card manufactuerer's website for more information.

Once downloaded and installed, you should be able to find the two required directories:

- include: This directory contains the master [CL.h] include file.
- lib: This directory contains the OpenCL libraries required at link time (OpenCL.lib / libOpenCL.a / ...).

## Windows

On Windows, OpenCL include and libs are not always shipped with the drivers. If that's your case, you have two options:

- Install the OpenCL include and libs from your graphic card manufacturer's website. It is usually available in their SDK.
- Use the shipped include/lib resources (located in the [deps] folder (not recommended as the shipped version may be outdated or not compatible).

### Manufacturer's version

Once installed (or extracted), locate the include and lib folders.
Open a terminal and create one environment variable per path. Then, install the :

```
set OPENCL_INCLUDE_PATH=<path to the [include] directory>
set OPENCL_LIB_PATH=<path to the [lib] directory>

# This will download and install the dependencies as well as build the plugin from its sources.
# To install the devlopment version, remove the [--production] flag.
npm --production install
```

### Embedded version

The embedded version is the default fallback build:

```
# This will download and install the dependencies as well as building the plugin from its sources.
# To install the devlopment version, remove the [--production] flag.
npm --production install
```

## Other (Linux, Mac OS, ...)

You can contact me to add compilation support for another platform.