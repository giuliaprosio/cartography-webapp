#pragma once

#include "structs.h"
#include <list>
#include <mutex>
#include <string>
#include <unordered_map>

extern std::unordered_map<std::string, GpsRecord> gpsRecords;
extern std::list<GpsRecord> listOfRecords;
inline std::mutex mtx;
inline std::mutex mtx_allRecords;

//GEOLOCATION DATA INSERT IN STRUCT
void insertGps(struct GpsRecord);

//GEOLOCATION DATA INSERT IN LIST OF ALL RECORDS
void insertGpsList(struct GpsRecord);
