#include "support_functions.h"

std::unordered_map<std::string, GpsRecord> gpsRecords = {};
std::list<GpsRecord> listOfRecords = {};


void insertGps(struct GpsRecord record){

    mtx.lock();
    gpsRecords[record.ipAddr] = record;
    mtx.unlock();

}

void insertGpsList(struct GpsRecord record){

    mtx_allRecords.lock();
    listOfRecords.push_back(record);
    mtx_allRecords.unlock();

}