cmake_minimum_required(VERSION 3.25)
project(webapp)

set(CMAKE_CXX_STANDARD 17)

set(CURL_LIBRARY "curl")
set(CURL_INCLUDE_DIR /home/giuli/Downloads/curl-8.1.2/include/)
find_package(CURL REQUIRED)
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lcurl")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl")

#include_directories(OpenSSL REQUIRED)
include_directories(/home/giuli/Downloads/asio/asio/include/)
include_directories(/home/giuli/Downloads/curl-8.1.2/include/)
include_directories(/home/giuli/Downloads/curl4cpp/)
include_directories(/home/giuli/Downloads/greensoft/output/build/buildroot-config/br2/package)
add_executable(webapp main.cpp support_functions.h server_ip_solver.cpp broadcasting_struct.cpp listener_udp.cpp geodata_insert.cpp)
#target_link_libraries(webapp OpenSSL)

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(webapp ${CURL_LIBRARIES})

set_target_properties(webapp
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out
)