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

//BROADCASTING FUNCTION
void broadcasting(struct GpsRecord); //struct GPSRecord