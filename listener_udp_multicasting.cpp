#include "geodata_insert.h"
#include "listener_udp_multicasting.h"
#include "server_ip_solver.h"
#include "utils.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>

void multicast_listener(){
    struct sockaddr_in localSock{};
    struct ip_mreq group{};
    int fd;

    //create a datagram socket on which to receive
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Couldn't create socket" << std::endl;
        return;
    }

    //enable SO_REUSE ADDRESS to allow multiple instances of this application
    //to receive copies of multicast datagrams
    {
        int reuse = 1;
        if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0){
            perror("error in setting reuse addr");
            exit(1);
        }
    }
    //bind to proper port
    memset((char *)&localSock, 0, sizeof(localSock));

    localSock.sin_family = AF_INET;
    localSock.sin_addr.s_addr = INADDR_ANY;
    localSock.sin_port = htons(getenvi("IN_PORT", 5550));

    //binding server side
    if (bind(fd, (const struct sockaddr *) &localSock, sizeof(localSock)) < 0) {
        std::cerr << "Couldn't bind" << std::endl;
        close(fd);
        return;
    }

    //join multicast group on local interface
    group.imr_multiaddr.s_addr = inet_addr("239.135.197.111");
    char localIP[256];
    thisServerIP(localIP);
    group.imr_interface.s_addr = inet_addr(localIP);
    if(setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0){
        perror("error in adding multicast group");
        close(fd);
        exit(1);
    }

    while(true){
        try{
            std::cout << "ready to receive" << std::endl;
            GpsRecord receivedRec{};

            int n;
            n = recvfrom(fd, (char *) &receivedRec, sizeof(receivedRec), MSG_WAITALL, NULL, NULL);
            if(n <= 0 ){
                //no client data to process
                std::cout << "no other user data to process";
                sleep(3);
                continue;

            }else{

                std::thread gpsOtherThread(insertGps, receivedRec);
                gpsOtherThread.detach();  //.join();
                std::thread gpsOtherThreadList(insertGpsList, receivedRec);
                gpsOtherThreadList.detach();
                continue;

            }
        }catch (std::exception &err) {
                std::cerr << "Generic error" << std::endl;
        }

    }
}

