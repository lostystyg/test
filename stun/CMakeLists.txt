
find_package(Boost REQUIRED)

add_executable(stun main.cpp)
target_include_directories(stun PUBLIC ${Boost_INCLUDE_DIRS})