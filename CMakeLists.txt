project (stun-test)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(client)
add_subdirectory(initiator)
add_subdirectory(stun)