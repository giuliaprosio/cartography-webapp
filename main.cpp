#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include "crow_all.h"
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include <list>
#include "curl4.hpp"

using namespace crow::json;

struct GpsRecord {

    //std::string ipAddr;
    char ipAddr[256];
    double lat;
    double lng;
    double acc;
    unsigned long lastSeen;
};

std::unordered_map<std::string, GpsRecord> gpsRecords;
std::list<GpsRecord> listOfRecords;


//GpsRecord rec;

std::mutex mtx;

void broadcastFunc(struct GpsRecord rec){
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

}

void insertGpsOther(std::string ipOther, double latOther, double lngOther, unsigned long lastSeenOther){
    GpsRecord otherRecord;
    //otherRecord.ipAddr = ipOther;
    std::string ipOfOther = ipOther; //req.remote_ip_address;
    strcpy(otherRecord.ipAddr, ipOfOther.c_str());
    otherRecord.lat = latOther;
    otherRecord.lng = lngOther;
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

void listenerFunction() {
    //creates a thread for each socket incoming
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

            struct sockaddr_in cliaddr;
            socklen_t len;
            int n;

            len = sizeof(cliaddr);
            n = recvfrom(socketFd, (char *) &recOfOtherUser, sizeof(recOfOtherUser), MSG_WAITALL,
                         (struct sockaddr *) &cliaddr, &len);
            if(n <= 0 ){ //no client data to process
                std::cout << "no other user data to process";
                sleep(2);
                continue;
            }else{
                std::string ipOfOther = (recOfOtherUser.ipAddr);
                double latOther = (recOfOtherUser.lat);
                double lngOther = (recOfOtherUser.lng);
                unsigned long timestampOther = (recOfOtherUser.lastSeen);

                std::cout << "client: " << sizeof(recOfOtherUser) << std::endl;
                std::cout << "client ip " << ipOfOther << std::endl;

                std::thread gpsOtherThread(insertGpsOther, ipOfOther, latOther, lngOther, timestampOther);
                gpsOtherThread.detach();  //.join();
                continue;
            }

        } catch (std::exception &err) {
            std::cerr << "Generic error" << std::endl;
        }
    }
}

int main()
{
    crow::App<crow::CORSHandler> app;

    CROW_ROUTE(app, "/")([](crow::response& res){
        res.set_static_file_info("static/index.html");
        res.end();
    });


    CROW_ROUTE(app, "/graphNetwork")([](const crow::request& req){
                        curl4::CURL4 init = curl4::easy::init();

                        std::string url = "https://localhost:443/admin/netgraph";

                        std::string xml;
                        struct curl_slist *headers = nullptr;
                        headers = curl_slist_append(headers, "Authorization: Basic YWRtaW46YjRtZXNocm9vdA==");
                        init.setopt(CURLOPT_URL, url);
                        init.setopt(CURLOPT_SSL_VERIFYPEER, 0);
                        init.setopt(CURLOPT_SSL_VERIFYHOST, 0);
                        init.setopt(CURLOPT_HTTPHEADER, headers);
                        init.setopt(CURLOPT_WRITEFUNCTION, curl4::easy::writefunc);
                        init.setopt(CURLOPT_WRITEDATA, &xml);

                        CURLcode res = curl4::easy::perform(init);

                        std::cout << res;

                        return crow::response{xml};


                    });

    CROW_ROUTE(app, "/gps")
            .methods("POST"_method)
                    ([](const crow::request& req){
                        auto payload = crow::json::load(req.body);
                        if (!payload)
                            return crow::response(400);

                        try {
                            GpsRecord rec;
                            //rec.ipAddr = payload["userIP"].s(); //req.remote_ip_address;
                            std::string ip = payload["userIP"].s(); //req.remote_ip_address;

                            strcpy(rec.ipAddr, ip.c_str());
                            //rec.ipAddr = ip;
                            rec.lat = payload["lat"].d();
                            rec.lng = payload["long"].d();
                            rec.acc = payload["acc"].d();
                            rec.lastSeen = payload["last_seen"].u();

                            while(true){
                                if(mtx.try_lock()){
                                    gpsRecords[rec.ipAddr] = rec;
                                    listOfRecords.push_back(rec);
                                    std::cout << "my GPS updated" << std::endl;

                                    std::thread threadBroadcastSend(broadcastFunc, rec);
                                    threadBroadcastSend.detach();
                                    mtx.unlock();
                                    break;
                                }else{
                                    sleep(2);
                                }
                            }

                        } catch (std::exception &err) {
                            CROW_LOG_INFO << "Error while parsing GPS record";
                        }

                        std::ostringstream os;
                        os << "";

                        return crow::response{os.str()};
                    });

    CROW_ROUTE(app, "/ip")([](const crow::request& req){
        auto &ip = req.remote_ip_address;
        return crow::response{ip};
    });


    CROW_ROUTE(app, "/gpsLastSeen")([](const crow::request& req){

        auto json = crow::json::wvalue::object{};

        for (auto &[key, val] : gpsRecords) {
            json[key] = crow::json::wvalue::object{
                    {"lat", val.lat}, {"lng", val.lng}, {"accuracy", val.acc}, {"ts", (std::uint64_t) val.lastSeen}
            };
        }

        return crow::response(crow::json::wvalue(json));
    });

    // router listen thread creation here
    try {
        std::cout << "creating a listener thread";
        std::thread threadListener(listenerFunction);
        threadListener.detach();
    }catch(std::exception &err){
        std::cout << "Can't create the thread" << std::endl;
    }

    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run_async();  //run_async() for asynchronous updates - useful with communication also
                                                        // on socket

}