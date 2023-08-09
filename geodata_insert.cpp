#include "support_functions.h"

std::unordered_map<std::string, GpsRecord> gpsRecords = {};
std::list<GpsRecord> listOfRecords = {};
std::mutex mtx;

void insertGpsOther(std::string ipOther, double latOther, double lngOther, double accOther, unsigned long lastSeenOther){
    GpsRecord otherRecord;

    std::string ipOfOther = ipOther;
    strcpy(otherRecord.ipAddr, ipOfOther.c_str());
    otherRecord.lat = latOther;
    otherRecord.lng = lngOther;
    otherRecord.acc = accOther;
    otherRecord.lastSeen = lastSeenOther;
    /*GpsRecord recOfOtherUser;

    const char *received = "Correctly received by server";

    struct sockaddr_in cliaddr;
    socklen_t len;
    int n;

    len = sizeof(cliaddr);
    n = recvfrom(socketFd, (char *)&recOfOtherUser, sizeof(recOfOtherUser), MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);

    std::string ipOfOther = (recOfOtherUser.ipAddr);
    std::cout << "client: " << sizeof(recOfOtherUser) << std::endl; */

    //int a = 0;
    while(true){
        if(mtx.try_lock()){

            gpsRecords[otherRecord.ipAddr] = otherRecord;
            listOfRecords.push_back(otherRecord);
            mtx.unlock();
            return;
        }else{
            //a++;
            sleep(2);
        }
        return;
    }

}