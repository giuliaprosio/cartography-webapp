#include "utils.h"
#include "support_functions.h"

void multicasting(struct GpsRecord rec) {

    struct in_addr localInterface;
    struct sockaddr_in groupSock;
    int fd;

    //create a datagram socket on which to send
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        std::cerr << "Couldn't create socket" << std::endl;
        return;
    }

    //initialize the group sockaddr structure  with group address GROUP and port number PORT
    memset((char *) &groupSock, 0, sizeof(groupSock));
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr("239.135.197.111");
    groupSock.sin_port = htons(getenvi("OUT_PORT", 5550));

    //NO LOOPBACK
    {
        char loopch=0;
        if(setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0){
            perror("error in setting no loopback option");
            close(fd);
            exit(1);
        }
    }

    //TTL 1
    {
        u_char ttl=1;
        if(setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl)) < 0){
            perror("error in setting ttl option");
            close(fd);
            exit(1);
        }
    }

    //set local interface for outbound multicast datagram

    char localIP[256];
    thisServerIP(localIP);
    localInterface.s_addr = inet_addr(localIP);
    if(setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0){
        perror("error in setting local interface");
        exit(1);
    }

    //send multicast message
    std::cout << "Sending multicast from local interface: " << localInterface.s_addr << std::endl;
    sendto(fd, (const char *)&rec, sizeof(rec), MSG_CONFIRM, (const struct sockaddr *)&groupSock, sizeof(groupSock));
    std::cout << "sent" << std::endl;  //let's see what's printing
    close(fd);
    std::cout << "closed" << std::endl;

    return;


}

