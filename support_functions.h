#pragma once

#include <unordered_map>
#include <string>
#include <mutex>
#include <list>

//struct GPS
struct GpsRecord {

    //std::string ipAddr;
    char ipAddr[256];
    double lat;
    double lng;
    double acc;
    char lastSeen[256];
};

extern std::unordered_map<std::string, GpsRecord> gpsRecords;
extern std::list<GpsRecord> listOfRecords;
inline std::mutex mtx;
inline std::mutex mtx_allRecords;

//BROADCASTING FUNCTION
void broadcasting(struct GpsRecord);

//MULTICASTING
void multicasting(struct GpsRecord);

//OWN IP ADDRESS RESOLVER
void thisServerIP(char *);

//INCOMING UDP LISTENER
void listener();

//INCOMING UDP MULTICAST LISTENER
void multicast_listener();

//GEOLOCATION DATA INSERT IN STRUCT
void insertGps(struct GpsRecord);

//GEOLOCATION DATA INSERT IN LIST OF ALL RECORDS
void insertGpsList(struct GpsRecord);
