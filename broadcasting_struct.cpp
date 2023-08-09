#include "support_functions.h"

//BROADCASTING FUNCTION
void broadcasting(struct GpsRecord rec){ //
    int portNum = 5555;

    int otherSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    int broadcastEnable = 1;

    if(otherSocketFd < 0){
        std::cerr << "Couldn't create socket" << std::endl;
        return;
    }

    int ret = setsockopt(otherSocketFd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if(ret < 0){
        std::cerr << "Error in setting broadcast option" << std::endl;
        return;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    inet_aton("255.255.255.255", &server_addr.sin_addr);
    //server_addr.sin_addr.s_addr = inet_addr("10.255.255.255");

    int n;
    socklen_t len;

    std::cout << "About to send data in broadcast" << server_addr.sin_addr.s_addr << std::endl;
    sendto(otherSocketFd, (const char *)&rec, sizeof(rec), MSG_CONFIRM, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    std::cout << "sent" << rec.lat << std::endl;  //let's see what's printing
    close(otherSocketFd);
    std::cout << "closed" << std::endl;

    return;
};
