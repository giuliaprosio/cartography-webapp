# Cartography

## Dependencies

- CMake
- libCURL
- libASIO

Install on Debian and derivatives:

```
sudo apt install cmake libcurl4-openssl-dev libasio-dev
```

## Build

```
mkdir build
cd build
cmake .. --DCMAKE_PREFIX=/usr/local
cmake --build .
```
