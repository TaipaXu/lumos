# 荧光闪烁

代码统计工具，便捷、快速！

[English](./README.md) | 中文

![lumos](./lumos.gif)

## 编译

在开始编译之前，请确保你已经安装了[vcpkg](https://github.com/microsoft/vcpkg)。设置`VCPKG_ROOT`环境变量，将其指向`vcpkg`的安装目录。将`vcpkg`的路径添加到系统的`PATH`环境变量中。
确保你已经安装了[CMake](https://cmake.org)与`pkg-config`，`libtool`。

```sh
git clone https://github.com/TaipaXu/lumos.git && cd lumos
vcpkg install
cmake --preset=unix-release
cmake --build build/release -j $(nproc)
```

# 许可证

[GPL-3.0](LICENSE)
