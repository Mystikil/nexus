# Nexus

## Building

This project uses [vcpkg](https://github.com/microsoft/vcpkg) to manage C++ dependencies. Configure the build with CMake and the vcpkg toolchain, for example:

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

The dependency list in `vcpkg.json` includes **boost-numeric-conversion** and **boost-algorithm**, so ensure these ports are available in your vcpkg installation before configuring.
