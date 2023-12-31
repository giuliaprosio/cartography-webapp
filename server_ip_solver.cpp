#include "server_ip_solver.h"
#include "utils.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ifaddrs.h>
#include <netdb.h>

//SERVER IP RESOLVER

void thisServerIP(char * host){
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char tmp[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    char *name = getenvs("BROADCAST_INTERFACE", "wlp3s0");

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),tmp, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if((strcmp(ifa->ifa_name,name)==0)&&(ifa->ifa_addr->sa_family==AF_INET))
        {
            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            printf("\tInterface : <%s>\n",ifa->ifa_name );
            strcpy(host, tmp);
            printf("\t  Address : <%s>\n", host);
        }
    }

    freeifaddrs(ifaddr);

}
