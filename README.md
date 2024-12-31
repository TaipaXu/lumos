# Lumos

Code statistics tool, convenient and fast!

English | [中文](./README_ZH.md)

![lumos](./lumos.gif)

## Dependencies

Before starting the compilation, please make sure you have installed [vcpkg](https://github.com/microsoft/vcpkg). Set the `VCPKG_ROOT` environment variable to point to the directory where `vcpkg` is installed. Add the `vcpkg` path to the system's `PATH` environment variable.

### Ubuntu

Make sure you have installed [CMake](https://cmake.org), `build-essential`, `pkg-config` and `libtool`.

## Compiling

### Linux & macOS

```sh
git clone https://github.com/TaipaXu/lumos.git && cd lumos
vcpkg install
cmake --preset=unix-release
cmake --build build/release -j $(nproc)
```

## Testing

```sh
cmake --preset=unix-debug -DENABLE_TESTS=ON
cmake --build build/debug -j $(nproc)
cmake --build build/debug --target test
```

## License

[GPL-3.0](LICENSE)
