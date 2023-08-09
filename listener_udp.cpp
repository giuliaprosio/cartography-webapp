#include "support_functions.h"

//LISTENER FUNCTION FOR INCOMING UDP

void listener(){
    int socketFd;
    int portNum = 5555;

    struct sockaddr_in servaddr;

    //create socket
    if ((socketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Couldn't create socket" << std::endl;
        return;
    }
    std::cout << "Done up to this point" << std::endl;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(portNum);

    //binding server side
    if (bind(socketFd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "Couldn't bind" << std::endl;
        return;
    }

    std::cout << "managed to bind" << std::endl;

    while (true){
        try {

            std::cout << "ready to receive from client" << std::endl;
            GpsRecord recOfOtherUser;

            //const char *received = "Correctly received by server";
            char host[NI_MAXHOST];
            thisServerIP(host);
            //----------------------------------------
            /*struct ifaddrs *ifaddr, *ifa;
            int family, s;


            if (getifaddrs(&ifaddr) == -1)
            {
                perror("getifaddrs");
                exit(EXIT_FAILURE);
            }


            for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
            {
                if (ifa->ifa_addr == NULL)
                    continue;

                s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

                if((strcmp(ifa->ifa_name,"wlp3s0")==0)&&(ifa->ifa_addr->sa_family==AF_INET))
                {
                    if (s != 0)
                    {
                        printf("getnameinfo() failed: %s\n", gai_strerror(s));
                        exit(EXIT_FAILURE);
                    }
                    printf("\tInterface : <%s>\n",ifa->ifa_name );
                    printf("\t  Address : <%s>\n", host);
                }
            } */
            //freeifaddrs(ifaddr);
            //---------------------------------------

            struct sockaddr_in cliaddr;
            socklen_t len;
            int n;

            len = sizeof(cliaddr);
            n = recvfrom(socketFd, (char *) &recOfOtherUser, sizeof(recOfOtherUser), MSG_WAITALL,
                         (struct sockaddr *) &cliaddr, &len);

            char* ipString = inet_ntoa(cliaddr.sin_addr);
            printf("current client IP: %s\n ", ipString);

            if(n <= 0 ){ //no client data to process
                std::cout << "no other user data to process";
                sleep(2);
                continue;
            }else if(ipString == host){

                continue;

            }else{

                std::string ipOfOther = (recOfOtherUser.ipAddr);
                double latOther = (recOfOtherUser.lat);
                double lngOther = (recOfOtherUser.lng);
                double accOther = (recOfOtherUser.acc);
                unsigned long timestampOther = (recOfOtherUser.lastSeen);

                std::cout << "client: " << sizeof(recOfOtherUser) << std::endl;
                std::cout << "client ip " << ipOfOther << std::endl;

                std::thread gpsOtherThread(insertGps, ipOfOther, latOther, lngOther, accOther, timestampOther);
                gpsOtherThread.detach();  //.join();
                continue;

            }

        } catch (std::exception &err) {
            std::cerr << "Generic error" << std::endl;
        }
    }
}