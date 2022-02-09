
find_package(Boost REQUIRED)

add_executable(client main.cpp)
target_include_directories(client PUBLIC ${Boost_INCLUDE_DIRS})