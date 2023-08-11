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
### Share location permission
Use the app on Chrome browser.
Allow giving your location in [chrome-devtools](chrome://flags/#unsafely-treat-insecure-origin-as-secure).

Enable application site: http:// ene5ai-yoi-0#.gnyoi.com:18080,
(# = router number).




