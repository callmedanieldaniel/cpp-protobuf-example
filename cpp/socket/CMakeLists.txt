cmake_minimum_required(VERSION 3.12)

project(server)

set(CMAKE_CXX_STANDARD 11)


find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIRS})

find_package(Threads REQUIRED)

include_directories( /opt/homebrew/Cellar/websocketpp/0.8.2)

add_executable(server server.cpp)

target_link_libraries(server
    Threads::Threads
     /opt/homebrew/Cellar/websocketpp/0.8.2
)