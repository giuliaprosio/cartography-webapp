#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include "crow_all.h"
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include <list>
#include <ifaddrs.h>


//struct GPS
struct GpsRecord {

    //std::string ipAddr;
    char ipAddr[256];
    double lat;
    double lng;
    double acc;
    unsigned long lastSeen;
};

extern std::unordered_map<std::string, GpsRecord> gpsRecords;
extern std::list<GpsRecord> listOfRecords;
//extern std::mutex mtx;

//BROADCASTING FUNCTION
void broadcasting(struct GpsRecord);

//OWN IP ADDRESS RESOLVER
void thisServerIP(char *);

//INCOMING UDP LISTENER
void listener();

//GEOLOCATION DATA INSERT IN STRUCT
void insertGpsOther(std::string, double, double, double, unsigned long);
