import * as backend from "./backend";
import * as ll from "./leafletLayers";
import * as loc from "./localize";

let LATITUDE = 0.0;
let LONGITUDE = 0.0;
let ACCURACY = 0.0;
let TIMESTAMP = 0.0;


window.onload = async () => {

    let map, layerGraph = ll.init();
    window.map = map;
    window.layer = layerGraph;

    parseGraph();
    setInterval(parseGraph, 10000); //10 seconds
    window.parseGraph = parseGraph;

    async function parseGraph() {

        navigator.geolocation.watchPosition(successCallback, errorCallback);

        let gpsLastSeen = await backend.getGPSLastSeen();
        let graph = await backend.getGraph();
        let serverIP = await backend.getServerIPAddress();
        let gpsAllRecords = await backend.getGPSRecords();
        console.log("gpslastseen: ", gpsLastSeen);
        console.log("graph: ", graph);
        //at this point I have both GPSLastSeen and graphxml

        //when timer finishes redoes everything
        //initialization to redo
        layerGraph.clearLayers();
        document.getElementById("routerList").innerHTML = "";

        //let idToIdx = {};


        /**NODES */
        let [absGraph, dropRouter] = loc.logicLocalization(gpsLastSeen, graph);
        console.log("to drop: ", dropRouter);

        //--------------------------

        for (let k = 0; k < absGraph.nodes.length; k++) {
            let absNode = absGraph.nodes[k];
            let absNodeIP = absNode.ip;
            let absNodeName = absNode.name;

            let popupContent = 'Name: ' + absNodeName + '<br>' + 'ip: ' + absNodeIP + '<br>';

            let absNodeType = absNode.type;
            if (absNodeType == 'user') {
                let c = ll.drawCircle(absNode.lat, absNode.lng, "#800000", 1.5, 1, 2);  //red
                let cAcc = ll.drawCircle(absNode.lat, absNode.lng, "red", absNode.acc, 0.5, 0);

                c.bindPopup(popupContent).openPopup();
                c.addTo(layerGraph);
                cAcc.bindPopup(popupContent).openPopup();
                cAcc.addTo(layerGraph);

                continue;
            }

            let r = ll.drawCircle(absNode.lat, absNode.lng, "black", 1.5, 1, 2);  //blue
            let rAcc = ll.drawCircle(absNode.lat, absNode.lng, "black", absNode.acc, 0.5, 0);

            r.bindPopup(popupContent).openPopup();
            r.addTo(layerGraph);
            rAcc.bindPopup(popupContent).openPopup();
            rAcc.addTo(layerGraph);

        }

        console.log("dropRouter: ", dropRouter);

        for (let m = 0; m < dropRouter.length; m++) {
            let nodeToDrop = dropRouter[m];
            const div = document.getElementById("routerList");
            let btn = div.appendChild(document.createElement("button"));
            btn.innerText = nodeToDrop.name; //to add button on click

            btn.onclick = function () {

                sendCoordinates(nodeToDrop.ip, LATITUDE, LONGITUDE, ACCURACY, TIMESTAMP);

                console.log("im dropping this router: ", nodeToDrop.ip);

                btn.remove();

                let msg = 'router dropped here: latitude: ' + LATITUDE + ', longitude: ' + LONGITUDE;
                document.getElementById("message").innerText = msg;
                document.getElementById("message").classList.add("shown");
                setTimeout(() => { document.getElementById("message").classList.remove("shown") }, 10000);

                let r = ll.drawCircle(LATITUDE, LONGITUDE, "black", 1.5, 1, 2);
                let rAcc = ll.drawCircle(LATITUDE, LONGITUDE, "black", ACCURACY, 0.5, 0);

                r.bindPopup(nodeToDrop.ip).openPopup();
                r.addTo(layerGraph);
                rAcc.bindPopup(nodeToDrop.ip).openPopup();
                rAcc.addTo(layerGraph); 

            }


        }


        /**EDGES */

        let alreadySeen = {};

        for (let l = 0; l < absGraph.edges.length; l++) {

            let edge = absGraph.edges[l];

            let source = edge.source;
            let dest = edge.dest;

            let key;
            if (source < dest) {
                key = source + "-" + dest;
            } else {
                key = dest + "-" + source;
            }

            let otherEdge = alreadySeen[key];
            if (otherEdge == undefined) {
                alreadySeen[key] = edge;
                continue;
            }

            //if(edge.rssi > -100 && otherEdge.rssi > -100){
            let polyl = ll.drawEdge(edge.sourceLat, edge.sourceLng, edge.destLat, edge.destLng);

            if (otherEdge) {
                let signal = 'Signal 1: ' + edge.rssi + '<br>' + 'Signal 2: ' + otherEdge.rssi + '<br>';
                polyl.bindPopup(signal).openPopup();
            }

            polyl.addTo(layerGraph);
            //}

           
            delete alreadySeen[key];



        }

    }



}

///SEND MESSAGE TO SERVER ALSO WHEN DROPPING A ROUTER

async function sendCoordinates(ip, lat, lng, acc, ts) {

    let json_coord = new Object();
    json_coord.userIP = ip;
    json_coord.lat = lat; //position.coords.latitude;
    json_coord.long = lng; //position.coords.longitude;
    json_coord.acc = acc; //position.coords.accuracy;
    json_coord.last_seen = ts; //position.timestamp;

    //this same thing also done when router dropped (line 178)
    let payload_coord = JSON.stringify(json_coord);
    let statusCode = backend.postGPSLastSeen(payload_coord);

    console.log(statusCode);

}

const successCallback = async (position) => {

    let myIP = await backend.getMyIPAddress(); 

    console.log("success");
    console.log(position);

    LATITUDE = Number(position.coords.latitude);
    LONGITUDE = Number(position.coords.longitude);
    ACCURACY = Number(position.coords.accuracy<= 50 ? position.coords.accuracy : 50.0); //
    TIMESTAMP = Number(position.timestamp);


    await sendCoordinates(myIP, LATITUDE, LONGITUDE, ACCURACY, TIMESTAMP);


};



const errorCallback = (error) => {
    console.log("error gps:", error);
};


//navigator.geolocation.watchPosition(successCallback, errorCallback, { timeout: 10000 }); //update every 30 seconds









