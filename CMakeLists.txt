cmake_minimum_required(VERSION 3.2)
project(mediator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${mediator_SOURCE_DIR}/build)

set(SOURCE_FILES)
add_executable(mediator ${SOURCE_FILES} serial_buzzergroup.cpp watchdog.cpp buzzergroup_manager.cpp)


target_link_libraries(mediator pthread boost_system)