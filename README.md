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

The following environment variables can be set to customize its behavior:

- `BROADCAST_INTERFACE`: The name of the interface to use for broadcasting the informations. (defaults to `wlp3s0`)
- `APP_PORT`: The port to use for serving the web app. (defaults to `18080`)
- `IN_PORT`: The port to use when listening over UDP. (defaults to `5550`)
- `OUT_PORT`: The port to use when sending data over UDP. (defaults to `5550`)
- `NETGRAPH_URL`: The url to the network graphml. (defaults to `"https://localhost:443/admin/netgraph"`)

It can be useful for running multiple server, for local testing purposes:

```bash
BROADCAST_INTERFACE=lo APP_PORT=18080 IN_PORT=7777 NETGRAPH_URL="file://$(realpath ../network/netgraph.xml)" ./webapp &
BROADCAST_INTERFACE=lo APP_PORT=18081 OUT_PORT=7777 ./webapp &
```

To kill the application, find process-id with command ps and execute
```bash
kill [process-id]
```

## Run local simulation for debug purposes

This will run two processes locally in a way so that they can exchange messages with each other.
The network topology Graphml is generated from the `debug/network.dot` to make it easier to edit.

To generate the Graphml you will need the following dependencies:

    sudo apt install graphviz python3-networkx

Then you can use this command tu run the debug simulation:

    ./debug.sh


## Use 
### Share location permission
Use the app on Chrome browser.
Allow giving your location possible in:
* **Chrome**, in chrome://flags/#unsafely-treat-insecure-origin-as-secure.
* **FireFox**, in devtools.serviceWorkers.testing.enabled setting in about:config

Enable application site: http:// ene5ai-yoi-0#.gnyoi.com:18080,
(# = router number).




