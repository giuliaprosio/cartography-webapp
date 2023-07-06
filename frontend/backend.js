import graphml from "graphml-js";

export async function getGPSLastSeen() {
   
    let gpsLastSeenJSON = await fetch("http://localhost:18080/gpsLastSeen", {
        method: "GET",
    });

    let gpsLastSeen = JSON.parse(await gpsLastSeenJSON.text());

    return gpsLastSeen;
}

export async function getGraph(){

    let graphNetwork = await fetch("http://localhost:18080/graphNetwork", {
        method: "GET",
    
    });
    
    let fileContent = await graphNetwork.text();

    let parser = new graphml.GraphMLParser();

    return new Promise((res, reject) => parser.parse(fileContent, function (err, graph) {
        if (err) reject(err);
        else res(graph);
    }))
}

export async function postGPSLastSeen(payload_coord){

    let res = await fetch("http://localhost:18080/gps", {
        method: "POST",
        body: payload_coord
    });

    let statusCode = res.status;

    return statusCode;
}

export async function getMyIPAddress(){
    let res = await fetch("http://localhost:18080/ip", {
        method: "GET", 
    });

    return await res.text();
}