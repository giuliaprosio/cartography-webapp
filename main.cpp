#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include "crow_all.h"

//socket programming

using namespace std;
using namespace crow::json;

struct GpsRecord {

    std::string ipAddr;
    double lat;
    double lng;
    double acc;
    unsigned long lastSeen;
};




std::unordered_map<std::string, GpsRecord> gpsRecords;

using gpsObject = std::unordered_map< std::string, wvalue >;
using gpsList = std::vector< wvalue >;

GpsRecord rec;

int main()
{
    crow::App<crow::CORSHandler> app;

    //define your endpoint at the root directory (app, url)
    CROW_ROUTE(app, "/")([](crow::response& res){
        res.set_static_file_info("static/index.html");
        res.end();
    });

    CROW_ROUTE(app, "/graphNetwork")([](crow::response& res){
                        res.set_static_file_info("network/netgraph.xml"); //netgraph
                        res.end();
                    });

    /*CROW_ROUTE(app, "/gpsLastSeen")([](crow::response& res){
        res.set_static_file_info("network/gpsPompier.json"); //netgraph
        res.end();
    }); */

    CROW_ROUTE(app, "/gps")
            .methods("POST"_method)
                    ([](const crow::request& req){
                        auto payload = crow::json::load(req.body);
                        if (!payload)
                            return crow::response(400);

                        try {

                            rec.ipAddr = payload["userIP"].s(); //req.remote_ip_address;
                            rec.lat = payload["lat"].d();
                            rec.lng = payload["long"].d();
                            rec.acc = payload["acc"].d();
                            rec.lastSeen = payload["last_seen"].u();

                            gpsRecords[rec.ipAddr] = rec;
                            //gpsObject [rec.ipAddr] = rec;

                            cout << rec.lat;

                            /*TODO: send and receive the struct to/from all the other routers in broadcast and ask in localhost to other server for xml
                             * both opening a socket
                             * 1_ localhost
                             * 2_ consider the broadcast address
                             *      2.1_ send GPSrecord rec in broadcast once you receive it from the user -- only if it's not the same as the one in the struct i have
                             *           already (match ip, lat, lng)
                             *      2.2_ add/update GPSrecord rec received from other router to GPSstruct to send to user
                             * all this done with socket so not Crow - also very problematic with with windows so it makes sense
                             * to work on it directly on ubuntu
                             *      */

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

    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();  //run_async() for asynchronous updates - useful with communication also
                                                 // on socket


}