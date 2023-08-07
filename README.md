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

    make TARGET=native

Or simply:

    make

Build for Greensoft routers:

    make TARGET=greensoft

If the Greensoft toolchain is compiled in another directory than `./greensoft` the `GREENSOFT_DIR` variable (or any other variable of the Makefile) can be overriden like so:

    make TARGET=greensoft GREENSOFT_DIR=../greensoft
