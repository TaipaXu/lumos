# Changelog

## Unreleased

- Optimize file scanning by short-circuiting known text extensions, sniffing their content, and only invoking libmagic when heuristics flag potential binaries.
- Enable aggressive compiler optimizations (-O3, -march=native, LTO) for faster binaries on supported toolchains.

## 0.0.1 - 2025-10-31

- Lumos Code statistics tool, convenient and fast!
