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

#### Prerequisites for C++ subproject for Linux

Install `vcpkg`:

```bash
$ cd ..
$ git clone https://github.com/Microsoft/vcpkg.git
$ cd vcpkg
$ ./bootstrap-vcpkg.sh

$ ./vcpkg install pybind11 doctest
```

To simplify `cmake` commands, `VCPKG_ROOT` enviromental variable can be
exported with the location of `vcpkg` root directory.

#### Prerequisites for C++ subproject for Windows

Install `vcpkg`:

```bash
cd ..
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat

vcpkg install pybind11 doctest --triplet windows-x64
```

To simplify `cmake` commands, `VCPKG_ROOT` enviromental variable can be
exported with the location of `vcpkg` root directory.

#### Build using `make`

```bash
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/vcpkg.cmake
$ cmake ..      # if VCPKG_ROOT is defined:
$ make  # by defaults builds in a release mode
$ make release
$ make debug
```

#### Build using `ninja`

However, the output from `cargo` will not be visible during building.

```bash
$ mkdir build
$ cd build
$ cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/vcpkg.cmake
$ cmake .. -G Ninja     # if VCPKG_ROOT is defined:
$ ninja   # by defaults builds in a release mode
$ ninja release
$ ninja debug
```

#### Prerequisites

You need a working Rust nightly compiler and maturin Python package.

The easiest way to install Rust compiler is to use [rustup](https://rustup.rs/).

Maturin can be installed using `pip`:

```bash
$ pip install maturin
```

#### Building binary wheel

The optimized binary wheel can be built using maturin:

```bash
$ maturin build --release --rustc-extra-args='-C target-cpu=native' --no-sdist --manylinux=off
```

or

```bash
$ cmake wheel
$ cmake wheel_install
```


### Unit testing

After building the project and installing the wheel, unit tests can be run
using:

```bash
$ ninja test
$ make test
$ ctest
'''

from `build/` directory.

From root directory, tests can be run using:

```bash
$ pytest va_py
```

