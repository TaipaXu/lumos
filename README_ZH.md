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

## 编译

### 依赖

在开始编译之前，请确保你已经安装了[vcpkg](https://github.com/microsoft/vcpkg)。设置`VCPKG_ROOT`环境变量，将其指向`vcpkg`的安装目录。将`vcpkg`的路径添加到系统的`PATH`环境变量中。

#### Ubuntu

确保你已经安装了[CMake](https://cmake.org)与`build-essential`，`pkg-config`，`libtool`。

### Linux & macOS

```sh
git clone https://github.com/TaipaXu/lumos.git && cd lumos
vcpkg install
cmake --preset=unix-release
cmake --build build/release -j $(nproc)
```

## 测试

```sh
cmake --preset=unix-debug -DENABLE_TESTS=ON
cmake --build build/debug -j $(nproc)
cmake --build build/debug --target test
```

## 协议

[GPL-3.0](LICENSE)
