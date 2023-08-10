import graphml from "graphml-js";

export async function getGPSLastSeen() {
   
    let gpsLastSeenJSON = await fetch("/gpsLastSeen", {
        method: "GET",
    });

    let gpsLastSeen = JSON.parse(await gpsLastSeenJSON.text());

    return gpsLastSeen;
}

export async function getGPSRecords() {

    let gpsRecordsJSON = await fetch("/gpsAllRecords", {
        method: "GET",
    });

    let gpsLastSeen = JSON.parse(await gpsRecordsJSON.text());

    return gpsRecordsJSON;
}

export async function getGraph(){

    let graphNetwork = await fetch("/graphNetwork", {
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

    let res = await fetch("/gps", {
        method: "POST",
        body: payload_coord
    });

    let statusCode = res.status;

    return statusCode;
}

export async function getMyIPAddress(){
    let res = await fetch("/ip", {
        method: "GET", 
    });

    return await res.text();
}

export async function getServerIPAddress(){
    let res = await fetch("/serverIP", {
        method: "GET",
    });

    return await res.text();
}