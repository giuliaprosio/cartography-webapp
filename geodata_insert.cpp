#include "support_functions.h"

std::unordered_map<std::string, GpsRecord> gpsRecords = {};
std::list<GpsRecord> listOfRecords = {};
extern std::mutex mtx;

void insertGps(struct GpsRecord record){

    while(true){
        if(mtx.try_lock()){
            gpsRecords[record.ipAddr] = record;
            listOfRecords.push_back(record);
            mtx.unlock();
            return;
        }else{
            sleep(2);
        }
        return;
    }

}