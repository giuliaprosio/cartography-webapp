#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include "crow_all.h"
#include <cstring>
#include <thread>
#include <list>
#include "curl4.hpp"
#include "support_functions.h"


using namespace crow::json;


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
                            GpsRecord rec{};
                            std::string ip = payload["userIP"].s();
                            strcpy(rec.ipAddr, ip.c_str());
                            std::cout << "user IP: " << rec.ipAddr << std::endl;
                            rec.lat = payload["lat"].d();
                            rec.lng = payload["long"].d();
                            rec.acc = payload["acc"].d();
                            std::string last_seen = payload["last_seen"].s();
                            strcpy(rec.lastSeen, last_seen.c_str());

                            std::cout << "TIMESTAMP TO SEND: " << rec.lastSeen << std::endl;

                            std::thread threadInsertUserData(insertGps, rec);
                            threadInsertUserData.detach();

                            std::thread threadIsertUserDataInList(insertGpsList, rec);
                            threadIsertUserDataInList.detach();

                            //BROADCASTING VERSION (USE MULTICAST)
                            //std::thread threadBroadcastSend(broadcasting, rec);
                            //threadBroadcastSend.detach();

                            std::thread threadMulticastSend(multicasting, rec);
                            threadMulticastSend.detach();

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

    CROW_ROUTE(app, "/serverIP")([](const crow::request& req){
        char serverIP[256];
        thisServerIP(serverIP);
        return crow::response{ serverIP };
    });


    CROW_ROUTE(app, "/gpsLastSeen")([](const crow::request& req){

        auto json = crow::json::wvalue::object{};

        mtx.lock();
        for (auto &[key, val] : gpsRecords) {
            json[key] = crow::json::wvalue::object{
                    {"lat", val.lat}, {"lng", val.lng}, {"accuracy", val.acc}, {"ts", val.lastSeen}
            };
        }
        mtx.unlock();

        return crow::response(crow::json::wvalue(json));
    });

    CROW_ROUTE(app, "/gpsAllRecords")([](const crow::request& req){

        std::list<GpsRecord>::iterator it;
        auto json = crow::json::wvalue::object{};
        int index = 0;

        mtx_allRecords.lock();
        for(it = listOfRecords.begin(); it != listOfRecords.end(); ++it){
            json[std::to_string(index)] = crow::json::wvalue::object{
                    {"ip", it -> ipAddr},
                    {"lat",      it -> lat},
                    {"lng",      it -> lng},
                    {"accuracy", it -> acc},
                    {"ts",       it -> lastSeen}
            };
            index ++;
        }
        mtx_allRecords.unlock();

        return crow::response(crow::json::wvalue(json));
    });

    // router listen thread creation here
    try {
        //BROADCASTING VERSION (USE MULTICAST)
        //std::cout << "creating a listener thread";
        //std::thread threadListener(listener);
        //threadListener.detach();

        std::cout << "creating multicast listener thread";
        std::thread multicast_threadListener(multicast_listener);
        multicast_threadListener.detach();
    }catch(std::exception &err){
        std::cout << "Can't create the thread" << std::endl;
    }



    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run_async();  //run_async() for asynchronous updates - useful with communication also
                                                        // on socket

}