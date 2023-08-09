#include "support_functions.h"

std::unordered_map<std::string, GpsRecord> gpsRecords = {};
std::list<GpsRecord> listOfRecords = {};
std::mutex mtx;

void insertGps(std::string ipOther, double latOther, double lngOther, double accOther, unsigned long lastSeenOther){
    GpsRecord otherRecord;

    std::string ipOfOther = ipOther;
    strcpy(otherRecord.ipAddr, ipOfOther.c_str());
    otherRecord.lat = latOther;
    otherRecord.lng = lngOther;
    otherRecord.acc = accOther;
    otherRecord.lastSeen = lastSeenOther;

    while(true){
        if(mtx.try_lock()){

            gpsRecords[otherRecord.ipAddr] = otherRecord;
            listOfRecords.push_back(otherRecord);
            mtx.unlock();
            return;
        }else{
            sleep(2);
        }
        return;
    }

}