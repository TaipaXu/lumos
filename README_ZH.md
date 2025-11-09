# 荧光闪烁

代码统计工具，便捷、快速！

[English](./README.md) | 中文

![lumos](./lumos.gif)

## 安装

### Ubuntu

```sh
sudo add-apt-repository ppa:taipa-xu/stable
sudo apt update
sudo apt install lumos
```

#### Debian 包

你也可以在 [GitHub Releases](https://github.com/TaipaXu/lumos/releases/tag/v0.0.1) 页面下载现成的 `.deb` 包。请选择与你的 Ubuntu 版本和架构匹配的文件，并使用 `sudo apt install ./<package-name>.deb` 进行安装。

| Ubuntu 版本 | 架构    | 下载 |
| ----------- | ------- | ---- |
| 22.04       | amd64   | [lumos_0.0.1-0ppa1.ubuntu22.04_amd64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu22.04_amd64.deb) |
| 22.04       | arm64   | [lumos_0.0.1-0ppa1.ubuntu22.04_arm64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu22.04_arm64.deb) |
| 24.04       | amd64   | [lumos_0.0.1-0ppa1.ubuntu24.04_amd64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu24.04_amd64.deb) |
| 24.04       | arm64   | [lumos_0.0.1-0ppa1.ubuntu24.04_arm64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu24.04_arm64.deb) |
| 25.04       | amd64   | [lumos_0.0.1-0ppa1.ubuntu25.04_amd64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu25.04_amd64.deb) |
| 25.04       | arm64   | [lumos_0.0.1-0ppa1.ubuntu25.04_arm64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu25.04_arm64.deb) |
| 25.10       | amd64   | [lumos_0.0.1-0ppa1.ubuntu25.10_amd64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu25.10_amd64.deb) |
| 25.10       | amd64v3 | [lumos_0.0.1-0ppa1.ubuntu25.10_amd64v3.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu25.10_amd64v3.deb) |
| 25.10       | arm64   | [lumos_0.0.1-0ppa1.ubuntu25.10_arm64.deb](https://github.com/TaipaXu/lumos/releases/download/v0.0.1/lumos_0.0.1-0ppa1.ubuntu25.10_arm64.deb) |

## 依赖

在开始编译之前，请确保你已经安装了[vcpkg](https://github.com/microsoft/vcpkg)。设置`VCPKG_ROOT`环境变量，将其指向`vcpkg`的安装目录。将`vcpkg`的路径添加到系统的`PATH`环境变量中。

### Ubuntu

确保你已经安装了[CMake](https://cmake.org)与`build-essential`，`libtool`。

### Windows

安装 [Visual Studio 2022](https://visualstudio.microsoft.com)，勾选“使用 C++ 的桌面开发”工作负载，并确保 [CMake](https://cmake.org) 已加入 `PATH`。请在“x64 Native Tools Command Prompt for VS 2022”或开发者 PowerShell 中执行以下命令，以便正确加载 MSVC 工具链。

## 编译

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

## 测试

```sh
cmake --preset=unix-debug -DENABLE_TESTS=ON
cmake --build build/debug -j $(nproc)
cmake --build build/debug --target test
```

## 协议

[GPL-3.0](LICENSE)
