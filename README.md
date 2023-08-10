# Cartography

## Dependencies

- CMake
- libCURL
- NodeJs + NPM

Install on Debian and derivatives:

    sudo apt install cmake libcurl4-openssl-dev

## Build

Build frontend and backend:

    make

Target platform can be set with the `TARGET` variable (default to `native`):

    TARGET=greensoft make

Build only frontend:

    make frontend

Build backend for current platform:

    make native

Build backend for Greensoft routers:

    make greensoft

If the Greensoft toolchain is compiled in another directory than `./greensoft-sdk` the `GREENSOFT_SDK_DIR` variable (or any other variable of the Makefile) can be overriden like so:

    make greensoft GREENSOFT_SDK_DIR=../greensoft
