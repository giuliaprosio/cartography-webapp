# Cartography

## Dependencies

- CMake
- libCURL

Install on Debian and derivatives:

```
sudo apt install cmake libcurl4-openssl-dev
```

## Build

```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build .
```
