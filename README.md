

## :scroll: Description 

Proof of concept of audio raytracing where we input the geometry 
of a simple room and an audio file, then it generates a `.bmp` file
showing the room along with a `.wav` audio file representing how the 
input soundfile will sound in the room.

There's a custom `.wav` and `.bmp` encoder/decoder that works 
both MacOS and in Windows.


## :desktop_computer: Building the project

Use cmake to build the project

```
mkdir build
cd build
cmake ..
```

If building for MacOS:

```
mkdir build
cd build
cmake -G Xcode ..
```

## :vulcan_salute: Results

### Results of the the current raytracer

![Image of results](https://blogger.googleusercontent.com/img/a/AVvXsEgvrRLs7jqRYjeSkjv64OIWeAUnZ51JX0Yn8yc3F_0AOrDhfFWyKtxTx8ccJBtKMXriwVPjQW247DsWcQOu7_VsJZXu5WdUeUw4xgPihWPX8k9ignhSr5fBQxTKcVlV8KaWM8BT9ElAoLEmsh9AV0ha03WUbfQMJ-DLXk0Rsi97hkoveUNtYNMoHVDNsQ8)

### Results of the wav encoder currently implemented

[![Watch the video](https://img.youtube.com/vi/j4xsWsei6AI/hqdefault.jpg)](https://www.youtube.com/watch?v=j4xsWsei6AI)


## :paperclips:	Third party libraries

- [Tinyobjloader](https://github.com/tinyobjloader/tinyobjloader): a single header library to load obj models.
- [OpenCL](https://github.com/KhronosGroup/OpenCL-SDK): pending implementation of OpenCL for Windows.
- [WAV Encoding](https://github.com/migueahumada/WAV-Decoder-Encoder): Uses a modified version of my encode/decode library.

> [!NOTE]
> This project is still in progress.
