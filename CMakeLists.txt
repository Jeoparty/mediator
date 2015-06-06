cmake_minimum_required(VERSION 3.2)
project(mediator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES)
add_executable(mediator ${SOURCE_FILES} serial_buzzergroup.cpp watchdog.cpp main.cpp buzzergroup_manager.cpp)

target_link_libraries(mediator pthread boost_system)