# Ring Buffer Cpp

A minimalist C++23 ring buffer.

## Features
- Raw dynamically allocated array to reduce reliance on stl.

## Prerequisites
- [Nix](https://nixos.org/download.html) (with flakes enabled)
- *Optional:* [direnv](https://direnv.net/)

## Development Setup

Enter the reproducible development shell (Clang):
```bash
nix develop
```
*(If using `direnv`, simply run `direnv allow`)*

Or pick a toolchain explicitly -- `CC`/`CXX` are set by the shell, so the
same CMake presets work in both:
```bash
nix develop .#clang
nix develop .#gcc
```

The Nix package builds follow the same scheme (`clang` is the default):
```bash
nix build           # Clang
nix build .#gcc     # GCC
```

Both toolchains ship `clangd`, `clang-format` and `clang-tidy`; the debugger
matches the compiler (`lldb` for Clang, `gdb` for GCC).

## Building

Configure the project using CMake presets:
```bash
cmake --preset default
```

Build the project (Debug by default):
```bash
cmake --build --preset debug
```

For a release build (optimized, no sanitizers):
```bash
cmake --build --preset release
```

## Testing

Run the test suite:
```bash
ctest --preset debug
```
