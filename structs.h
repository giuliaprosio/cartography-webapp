#pragma once

//struct GPS
struct GpsRecord {

    //std::string ipAddr;
    char ipAddr[256];
    double lat;
    double lng;
    double acc;
    char lastSeen[256];
};
