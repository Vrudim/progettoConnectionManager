cmake_minimum_required(VERSION 3.20)

project(TcpClientServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server src/main_server.cpp)
add_executable(client src/main_client.cpp)

target_link_libraries(server PRIVATE ws2_32)
target_link_libraries(client PRIVATE ws2_32)