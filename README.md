# 2048 cpp game

## Introduction
2048 is a single-player sliding tile puzzle video game written by Italian web developer Gabriele Cirulli and published on GitHub [wikipedia](https://en.wikipedia.org/wiki/2048_(video_game)).

## Supported Platforms
2048 cpp game supports only Linux system from Ubuntu 22.04 LTS version.

## Dependencies
This repository is depend on vary libraries. These are located in the `external` directory.

- [GLFW](https://github.com/glfw/glfw) for provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling events, etc.
- [Dear ImGui](https://github.com/ocornut/imgui) is bloat-free graphical user interface library for C++
- [spdlog](https://github.com/gabime/spdlog) is very fast, header-only/compiled, C++ logging library
- [gRPC](https://github.com/grpc/grpc) is remote procedure call (RPC) framework that can run anywhere. gRPC enables client and server applications to communicate transparently, and simplifies the building of connected systems.
- [SQLite](https://www.sqlite.org/) is a C-language library that implements a small, fast, self-contained, high-reliability, full-featured, SQL database engine

### Build and locally install gRPC and Protocol Buffers

The steps in the section refer [the official page](https://grpc.io/docs/languages/cpp/quickstart/). If you don't want to read them all, please following these steps below to set up quickly:

```
export MY_INSTALL_DIR=$HOME/.local
mkdir -p $MY_INSTALL_DIR
export PATH="$MY_INSTALL_DIR/bin:$PATH"
sudo apt install -y cmake git
sudo apt install -y build-essential autoconf libtool pkg-config
git clone --recurse-submodules -b v1.66.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
cd grpc
mkdir -p cmake/build
pushd cmake/build
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
make -j8
make install
popd
cd ..
```

### Install dependencies for ImGui
The steps in the section refer [the official page](https://www.glfw.org/docs/latest/compile_guide.html). If you don't want to read them all, please following these steps below to set up quickly:

```
sudo apt install -y libwayland-dev libxkbcommon-dev xorg-dev
```

### Install dependencies for SQLite

```
sudo apt install -y libsqlite3-dev
```

## How to build and usage

Clone the main repository and build it.

```
git clone --recurse-submodules https://github.com/chibihate/2048-cpp.git
cd 2048-cpp
mkdir build
cd build
cmake -DDEBUG=ON ..
make -j8
```

Run `Server` to initialize server to receive and calculate data from `Client`.
```
./Server
```
Open another terminal to run `Client`, put `username` and `password` for the first time.
```
./Client
```

Although all data is recorded in database, and this is a demo, `Server` only show `username` active when `Server` start.

