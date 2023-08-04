cmake_minimum_required(VERSION 3.25)
project(webapp)

set(CMAKE_CXX_STANDARD 17)

# Add cmake folder to CMAKE_MODULE_PATH
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(CURL REQUIRED)
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lcurl")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl")

#include_directories(OpenSSL REQUIRED)
# include_directories(/home/giuli/Downloads/asio/asio/include/)
# include_directories(/home/giuli/Downloads/curl-8.1.2/include/)
# include_directories(/home/giuli/Downloads/curl4cpp/)
# include_directories(/home/giuli/Downloads/greensoft/output/build/buildroot-config/br2/package)
find_package(ASIO REQUIRED)

include(FetchContent)
# Fetch library from its git repository
FetchContent_Declare(
        curl4cpp
        GIT_REPOSITORY https://github.com/ferhatgec/curl4cpp
        GIT_TAG        962c77366f4c6793d04dc4f963adfe8dc0c73c7b # latest master
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
)
FetchContent_MakeAvailable(curl4cpp)
add_library(curl4cpp INTERFACE)
target_include_directories(curl4cpp INTERFACE ${curl4cpp_SOURCE_DIR})

add_executable(webapp main.cpp)
#target_link_libraries(webapp OpenSSL)

# include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(webapp ${CURL_LIBRARIES} curl4cpp)

set_target_properties(webapp
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out
)
