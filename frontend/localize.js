export function logicLocalization(gps, graph) {
    let absGraph = {
        "nodes": [],
        "edges": []
    };

    let toDrop = [];
    let mapOfIdx = {};

    /**
     * abs
     */
    let userNodeIds = [];
    let alreadyDroppedRouters = [];

    /**NODES */
    for (let i = 0; i < graph.nodes.length; i++) {

        let absNode = {};

        let node = graph.nodes[i];
        let nodeIP = node._attributes["main-ip-addr"];

        if (gps[nodeIP]) {
            absNode.name = node._attributes.hostname;
            absNode.ip = nodeIP;
            absNode.lat = gps[nodeIP].lat;
            absNode.lng = gps[nodeIP].lng;
            absNode.acc = gps[nodeIP].accuracy;
            absNode.ts = gps[nodeIP].ts.toString();

            if (node._attributes.type === 'station') {
                absNode.type = "user";
                let associatedRouterId = associatedRouter(node._id, graph.edges);
                let routerIdx = associatedRouterId.match(/\d+/)[0];
                absNode.associatedRouterName = graph.nodes[routerIdx]._attributes.hostname.substring(7,13);
                absNode.associateRouterIP = graph.nodes[routerIdx]._attributes["main-ip-addr"];
                userNodeIds.push(node._id);

                for (let edge of graph.edges) {
                    if (edge._target === node._id) {
                        mapOfIdx[edge._source] = absGraph.nodes.length;
                    }
                }
            } else {
                absNode.type = "router";
                mapOfIdx[node._id] = absGraph.nodes.length;
                alreadyDroppedRouters.push(node._id);
            }
            absGraph.nodes.push(absNode);

        }
    }

    // now, userNodeIds is completed, so we can do edge router check properly

    for (let i = 0; i < graph.nodes.length; i++) {
        let absNode = {};

        let node = graph.nodes[i];
        let nodeIP = node._attributes["main-ip-addr"];

        if (!userNodeIds.includes(node._id) && !isEdgeRouter(node, userNodeIds, graph.edges) && !alreadyDroppedRouters.includes(node._id) && (node._attributes.type !== 'station') && (node._attributes.type !== 'gate')) {

        // otherwise, it's a droppable router
        //create button
            let dropRouter = {};
            dropRouter.ip = nodeIP;
            let stringName = String(node._attributes.hostname);
            let substringName1 = stringName.substring(7, 13);
            //let substringName2 = stringName.substring(11, 12);
            let name = substringName1; //.concat(substringName2);
            dropRouter.name = name;
            toDrop.push(dropRouter);
        }
    }

    /**EDGES */
    for (let edge of graph.edges) {

        let absEdge = {};
        if (mapOfIdx[edge._source] !== undefined && mapOfIdx[edge._target] !== undefined) {

            absEdge.source = mapOfIdx[edge._source];
            absEdge.dest = mapOfIdx[edge._target];
            absEdge.rssi = edge._attributes["max-signal"];


            for (let p = 0; p < absGraph.nodes.length; p++) {
                let nodeForCoord = absGraph.nodes[p];
                if (absEdge.source === p) {
                    absEdge.sourceLat = nodeForCoord.lat;
                    absEdge.sourceLng = nodeForCoord.lng;

                } else if (absEdge.dest === p) {
                    absEdge.destLat = nodeForCoord.lat;
                    absEdge.destLng = nodeForCoord.lng;
                }
            }

            absGraph.edges.push(absEdge);
        }


    }

    console.log("edges to be drawn: ", absGraph.edges);



    return [absGraph, toDrop];
}

// is_edge_router iff exists connected node: station
function isEdgeRouter(routerNode, userNodeIds, graphEdges) {
    for (let userNodeId of userNodeIds) {
        for (let edge of graphEdges) {
            if (routerNode._id === edge._source & userNodeId === edge._target) {
                return true;
            }
        }
    }
    return false;
}

function associatedRouter(userNode, edges){
    for(let edge of edges){
        if(userNode === edge._target){
            return edge._source;
        }
    }

    return null;
}

