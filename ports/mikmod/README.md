# W3MikMod
A Win32 GUI for MikMod designed to run on win32s.

![w3mikmod](/assets/w3mikmod.png)

## Building
To build this, you need the following:
* GNU make or Watcom make
* OpenWatcom (tested on 2.0 beta)

## Prerequistes
* Windows 3.1
* Microsoft Win32s 1.30c (might work in other versions, not tested, compatible comctl32.dll required)
* Also works in any later version, and wine.

## Running
* An optional list of modules can be passed on command line

## w3mikmod.ini settings
* font: font name to use. fixed-width font assumed.
* size: font size
* empty: show empty samples/instruments names in their respective windows
* avewidth: use font average character width instead of max to resize samples/instruments windows
