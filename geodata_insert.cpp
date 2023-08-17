#include "support_functions.h"

std::unordered_map<std::string, GpsRecord> gpsRecords = {};
std::list<GpsRecord> listOfRecords = {};


void insertGps(struct GpsRecord record){

    mtx.lock();
    gpsRecords[record.ipAddr] = record;
    mtx.unlock();

    /*while(true){
        if(mtx.try_lock()){
            gpsRecords[record.ipAddr] = record;
            mtx.unlock();
            break;
        }else{
            sleep(2);
            continue;
        }
    }*/

}

void insertGpsList(struct GpsRecord record){

    mtx_allRecords.lock();
    listOfRecords.push_back(record);
    mtx_allRecords.unlock();

    /*while(true){
        if(mtx_allRecords.try_lock()){
            listOfRecords.push_back(record);
            mtx_allRecords.unlock();
            break;
        }else{
            sleep(2);
            continue;
        }
    } */
}