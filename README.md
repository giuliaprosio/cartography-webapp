# Cartography

## Additional configuration of the toolchain
In the **greensoft-sdk** folder execute:
```bash
make menuconfig
```
A prompt window should pop-up.

In Menu Target packages ----> Libraries ----> Networking
Select:
* Libcurl

This will allocate the folder for the libcurl package.
Then always in the same folder execute:
```bash
make libcurl-rebuild
```

Now the package will be correctly added in the toolchain.

The installed version is too old for the application, so search inside the folder for the
file **libcurl.mk** and change at the top
"LIBCURL_VERSION" to 8.1.2 
Note: you will need internet connection
for that.

Finally in the file **libcurl.hash** add the following line:

"sha256  31b1118eb8bfd43cd95d9a3f146f814ff874f6ed3999b29d94f4d1e7dbac5ef6  curl-8.1.2.tar.xz "


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

This way in the folder 'out' with the executable file will be generated.

Execute inside the /frontend folder of the application the command

```bash
npm run build:dev
```

To generate the 'static' folder associated with the executable.

## Upload in the router

First create in the router a writable folder to put the application in.

```bash
ssh -X default@IP
cd /var/persistent-data
sudo mkdir cartographyApp
sudo chown -R default:www-data /var/persistent-data/cartographyApp
```

Then copy the folder containing the executable and the static files in the router

```bash
scp -r ./out default@IP:/var/persistent-data/cartographyApp
```

## Start and kill
To make the application start, ssh in the router, change directory to out and execute the command
```bash
./webapp &
```

To kill the application, find process-id with command ps and execute
```bash
kill [process-id]
```

## Use 


