cmake_minimum_required(VERSION 3.25)
project(webapp)

set(CMAKE_CXX_STANDARD 17)

include_directories(/home/giuli/Downloads/asio/asio/include/)
add_executable(webapp main.cpp)