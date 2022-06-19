# CoreMediaIO-DAL-Example

Modernized and extended version of Apple's CoreMediaIO sample code.

### Before you jump right in

Apple announced the **deprecation of CMIO DAL plugins** at WWDC 2022, sunsetting them definitely in the next major release after macOS 13 (that is, in macOS 14). As a replacement, [Camera Extensions](https://developer.apple.com/documentation/coremediaio/creating_a_camera_extension_with_core_media_i_o) have been introduced in macOS 12.3 which are now the preferred way to develop camera plugins. If you are about to start development, do yourself a favor: drop support for anything below macOS 12.3 and **don't waste your time with DAL plugins**.

If you are still here, read on.

### Description

This repository started off as a fork of the official [CoreMediaIO sample code](https://developer.apple.com/library/archive/samplecode/CoreMediaIO/Introduction/Intro.html) by Apple. Unfortunately, the original code didn't age well since it was last updated in 2012 so here you can find the necessary changes to make it compile on modern systems.

### Virtual camera support

The original sample was mainly focused around camera plugins backed by real hardware which may be of little interest to anyone who is not a camera manufacturer. So eventually the code was extended with virtual camera support, i.e. DAL plugins that appear and can act like video sources but have no accompanying hardware. These cameras may draw their frames from whatever source they please, including but not limited to baked-in still images, content generated on-the-fly by the plugin itself, and relayed frames originating from a user-space companion application. (Note that the present code only implements reading from a static framestore.)

### Known issues

- the kext is not being maintained and is most probably dysfunctional
- IPC between the plugin and assistant is still built around the obsolete bootstrap server, this should be replaced by XPC
- and many more, see the [Issues tab](https://github.com/lvsti/CoreMediaIO-DAL-Example/issues) (and help reporting them if you come across one)

### Requirements to build

Xcode 10.1+

Note: the sample YUV files in the [KernelExtension](https://github.com/lvsti/CoreMediaIO-DAL-Example/tree/master/Sources/Extras/CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/KernelExtension) folder have been removed from the repo because the increased traffic continuously exhausted my LFS quota. From now on, please refer to the official [CoreMediaIO sample code](https://developer.apple.com/library/archive/samplecode/CoreMediaIO/Introduction/Intro.html) bundle which also contains these files.

### Contributing

The goal of this project is to maintain an up-to-date and relevant sample code for those who have just made first contact with the CoreMediaIO DAL world. Contributions along these lines are warmly welcome. Improvements, however, that are introducing a new feature or would steer the project away from its general/educational purpose would probably best be placed in a different fork.

### License

MIT
