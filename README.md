# LibreEMS - Open Source Engine Management

### Introduction

LibreEMS - The open source engine management system! This is a fork from the 
FreeEMS project (freeems.org)

Please see [LibreEMS.org](http://LibreEMS.org) for the most up to date information and links!

To casually follow the highlights you can subscribe to our [facebook page](http://www.facebook.com/OfficialLibreEMS)!

If you're wondering why this project was forked from FreeEMS have a look at [this page](http://blogs.libreems.org/why-did-we-fork-from-freeems/)

### Obtaining the firmware

The best way to get the latest development firmware is from the [build server!](http://builds.libreems.org/Firmware)

### Building the firmware

If you want to build your own firmware for some reason such as customisations, read below.

Ensure that you have the latest release or latest code from the [github repository](https://git.libreems.org/libreems-suite/libreems-firmware.git).

We have pre-packed debian/ubuntu toolchain binaries available at the [apt repository site](https://apt.libreems.org).

Toolchain binaries for 32 bit and 64 bit windows as well as Mac OS-x (10.6.8 or newer) at the [build site](https://builds.libreems.org/ToolChains).

If you don't have a debian/ubuntu/mac/windows system and want to make your toolchain by hand you can get the source/patches at the [build site](https://builds.libreems.org/ToolChains/Source).

You also need an up to date version of git available on the command line.

With these prerequisites met, simply type 'make' and choose an option!

### Loading the firmware

**Please note**: hcs12mem is no longer capable of loading the firmware unassisted.

You need either LibreEMS Loader (s12x-loader) or mtxloader. The former is a standalone app, and the latter is packaged with MTX.


If you are working on the dev board, you need to flip the load/run switch
to "load" and depress the reset button, or send a reset packet, prior to
flashing.  Return the switch to the "run" position when complete.

### Tuning your LibreEMS Install

Tuning is currently done with a combination of techniques. For the full story
check out [this thread](https://forums.libreems.org/forumdisplay.php?fid=8)
and the threads that it links to.

### Footnote

Please post any and all feedback on code, documentation, the system, anything
at all, even subtle and minor stuff to [the forum](https://forums.libreems.org).

Thanks for playing with LibreEMS :-)

Good luck and regards,

## LibreEMS Team.

