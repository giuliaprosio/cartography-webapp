# Cartography

## Dependencies

- CMake
- libCURL

Install on Debian and derivatives:

```
sudo apt install cmake libcurl4-openssl-dev
```

## Build

Build for current platform:

    make native

Or simply:

    make

Build for Greensoft routers:

    make greensoft

If the Greensoft toolchain is compiled in another directory than `./greensoft-sdk` the `GREENSOFT_SDK_DIR` variable (or any other variable of the Makefile) can be overriden like so:

    make greensoft GREENSOFT_SDK_DIR=../greensoft
