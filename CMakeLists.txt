cmake_minimum_required(VERSION 3.25)
project(webapp)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

# Add libcurl
find_package(CURL REQUIRED)

# Add curl4cpp header only library
FetchContent_Declare(curl4cpp
        GIT_REPOSITORY https://github.com/ferhatgec/curl4cpp
        GIT_TAG        962c77366f4c6793d04dc4f963adfe8dc0c73c7b # latest master
        CONFIGURE_COMMAND ""
        BUILD_COMMAND "")
FetchContent_MakeAvailable(curl4cpp)
add_library(curl4cpp INTERFACE)
target_include_directories(curl4cpp INTERFACE ${curl4cpp_SOURCE_DIR})

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
find_package(Threads)
target_link_libraries(asio INTERFACE Threads::Threads)

add_executable(webapp main.cpp)

target_link_libraries(webapp ${CURL_LIBRARIES} curl4cpp asio)

set_target_properties(webapp
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)
