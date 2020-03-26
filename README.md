# Volume Augmentations

This project includes a Python library that allows to efficiently augment 3D volumes. The augmentations implemented are:

* rotation,
* translation,
* scaling,
* multiplying whole volume by a value,
* adding a value to the whole volume.

## Getting Started

These instructions will allow you to use the library in your Python project.

### Installing precompiled wheels

Precompiled wheels for different versions of Python are automatically built for every release.

Install them with `pip`:

```bash
pip install <name_of_the_version>.whl
```

### Compiling from source

#### Prerequisites for C++ subproject 

Install `vcpkg`:

```bash
$ cd ..
$ git clone https://github.com/Microsoft/vcpkg.git
$ cd vcpkg
$ ./bootstrap-vcpkg.sh

$ ./vcpkg install pybind11
```

$$$$ Build using `make`

```bash
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/vcpkg.cmake
$ make  # by defaults builds in a release mode
$ make release
$ make debug
```


$$$$ Build using `ninja`

However, the output from `cargo` will not be visible during building.

```bash
$ mkdir build
$ cd build
$ cmake .. -GNinja -DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/vcpkg.cmake
$ ninja   # by defaults builds in a release mode
$ ninja release
$ ninja debug
```


#### Prerequisites

You need a working Rust nightly compiler and maturin Python package.

The easiest way to install Rust compiler is to use [rustup](https://rustup.rs/).

Maturin can be installed using `pip`:

```bash
pip install maturin
```

#### Building binary wheel

The optimized binary wheel can be built using maturin:

```bash
maturin build --release --rustc-extra-args='-C target-cpu=native' --no-sdist --manylinux=off
```
