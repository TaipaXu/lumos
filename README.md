# Lumos

Code statistics tool, convenient and fast!

English | [中文](./README_ZH.md)

![lumos](./lumos.gif)

## Installation

### Ubuntu

```sh
sudo add-apt-repository ppa:taipa-xu/stable
sudo apt update
sudo apt install lumos
```

#### Debian Packages

Prebuilt `.deb` archives are also available on the [GitHub Releases](https://github.com/TaipaXu/lumos/releases/tag/v0.0.1) page. Download the package that matches your Ubuntu version and architecture, then install it with `sudo apt install ./<package-name>.deb`.

| Ubuntu Version | Architecture | Download |
| -------------- | ------------ | -------- |
| 22.04          | amd64        | [lumos_0.0.1-0ppa1.ubuntu22.04_amd64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu22.04_amd64.deb) |
| 22.04          | arm64        | [lumos_0.0.1-0ppa1.ubuntu22.04_arm64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu22.04_arm64.deb) |
| 24.04          | amd64        | [lumos_0.0.1-0ppa1.ubuntu24.04_amd64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu24.04_amd64.deb) |
| 24.04          | arm64        | [lumos_0.0.1-0ppa1.ubuntu24.04_arm64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu24.04_arm64.deb) |
| 25.04          | amd64        | [lumos_0.0.1-0ppa1.ubuntu25.04_amd64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu25.04_amd64.deb) |
| 25.04          | arm64        | [lumos_0.0.1-0ppa1.ubuntu25.04_arm64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu25.04_arm64.deb) |
| 25.10          | amd64        | [lumos_0.0.1-0ppa1.ubuntu25.10_amd64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu25.10_amd64.deb) |
| 25.10          | amd64v3      | [lumos_0.0.1-0ppa1.ubuntu25.10_amd64v3.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu25.10_amd64v3.deb) |
| 25.10          | arm64        | [lumos_0.0.1-0ppa1.ubuntu25.10_arm64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu25.10_arm64.deb) |

## Dependencies

Before starting the compilation, please make sure you have installed [vcpkg](https://github.com/microsoft/vcpkg). Set the `VCPKG_ROOT` environment variable to point to the directory where `vcpkg` is installed. Add the `vcpkg` path to the system's `PATH` environment variable.

### Ubuntu

Make sure you have installed [CMake](https://cmake.org), `build-essential` and `libtool`.

### Windows

Install [Visual Studio 2022](https://visualstudio.microsoft.com) with the "Desktop development with C++" workload and make sure [CMake](https://cmake.org) is available in your `PATH`. Run the build commands from the "x64 Native Tools Command Prompt for VS 2022" (or a developer PowerShell prompt) so the MSVC toolchain is configured.

## Compiling

### Linux & macOS

```sh
git clone https://github.com/TaipaXu/lumos.git && cd lumos
vcpkg install
cmake --preset=unix-release
cmake --build build/release -j $(nproc)
```

### Windows

```powershell
git clone https://github.com/TaipaXu/lumos.git && cd lumos
vcpkg install
cmake --preset=msvc-release
cmake --build build\release --config Release -- /m
```

## Testing

```sh
cmake --preset=unix-debug -DENABLE_TESTS=ON
cmake --build build/debug -j $(nproc)
cmake --build build/debug --target test
```

## License

[GPL-3.0](LICENSE)
