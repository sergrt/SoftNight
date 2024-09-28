# SoftNight
SoftNight color control

## Description
Tiny and efficient no-nonsence Windows utility to control color temperature and brightness.

There are some utilities to control color temperature in the wild (like f.lux or SunsetScreen). Some of them cause flicker on screen, some even want money. So here's very simple, fast and free solution to control color temperature and brightness. Two sliders, two time points, bunch of hotkeys and that's it.
- Simple and easy on resources
- Switch day/night presets based on time 
- Hotkeys support for fast temperature/brightness adjustments

## Download
Use `Releases` section to download latest build

## Build

1. `$ git clone --recurse-submodules https://github.com/sergrt/SoftNight.git`
2. Build `3rdparty/wxWidgets` with your compiler
3. `$ cd SoftNight`
4. `$ mkdir build`
5. `$ cd build`
6. `% cmake ..`
7. Build generated solution with your compiler

Note: `wsWidgets` can be obtained as prebuilt binaries. If you want to use that, then
1. clone just `https://github.com/sergrt/SoftNight.git`
2. Clone `json` : `$ git clone --depth 1 --branch v3.11.3 --recurse-submodules https://github.com/nlohmann/json.git`
3. Generate solution with `cmake` and build

Icon credits goes to <a href="https://www.flaticon.com/free-icons/night" title="night icons">Night icons created by Freepik - Flaticon</a>
