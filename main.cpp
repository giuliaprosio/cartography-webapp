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
                            GpsRecord rec;
                            std::string ip = payload["userIP"].s();
                            strcpy(rec.ipAddr, ip.c_str());
                            std::cout << "user IP: " << rec.ipAddr << std::endl;
                            rec.lat = payload["lat"].d();
                            rec.lng = payload["long"].d();
                            rec.acc = payload["acc"].d();
                            rec.lastSeen = payload["last_seen"].u();

                            std::thread threadInsertUserData(insertGps, rec);
                            threadInsertUserData.detach();

                            std::thread threadBroadcastSend(broadcasting, rec);
                            threadBroadcastSend.detach();

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
        std::thread threadListener(listener);
        threadListener.detach();
    }catch(std::exception &err){
        std::cout << "Can't create the thread" << std::endl;
    }

    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run_async();  //run_async() for asynchronous updates - useful with communication also
                                                        // on socket

}