cmake_minimum_required(VERSION 3.22.1)
project(webapp)
include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # Let other programs know how to compile
set(CMAKE_CXX_STANDARD 17)

# Add curl4cpp header only library
FetchContent_Declare(curl4cpp
        GIT_REPOSITORY https://github.com/ferhatgec/curl4cpp
        GIT_TAG        962c77366f4c6793d04dc4f963adfe8dc0c73c7b # latest master
        CONFIGURE_COMMAND ""
        BUILD_COMMAND "")
FetchContent_MakeAvailable(curl4cpp)
add_library(curl4cpp INTERFACE)
target_include_directories(curl4cpp INTERFACE ${curl4cpp_SOURCE_DIR})

# Make sure CURL package is there and link it to asio
find_package(CURL REQUIRED)
target_link_libraries(curl4cpp INTERFACE ${CURL_LIBRARIES})

# Add asio header only library
FetchContent_Declare(asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio
        GIT_TAG        asio-1-28-1
        CONFIGURE_COMMAND ""
        BUILD_COMMAND "")
FetchContent_MakeAvailable(asio)
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)

# Make sure Threads package is there and link it to asio
find_package(Threads REQUIRED)
target_link_libraries(asio INTERFACE Threads::Threads)


add_executable(webapp main.cpp support_functions.h server_ip_solver.cpp broadcasting_struct.cpp listener_udp.cpp geodata_insert.cpp multicasting_struct.cpp listener_udp_multicasting.cpp)

target_link_libraries(webapp curl4cpp asio)

set_target_properties(webapp
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)
