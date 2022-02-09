
find_package(Boost REQUIRED)

add_executable(initiator main.cpp)
target_include_directories(initiator PUBLIC ${Boost_INCLUDE_DIRS})