cmake_minimum_required(VERSION 2.8)
project(mediator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${mediator_SOURCE_DIR}/build)

set(SOURCE_FILES serial_buzzergroup.cpp watchdog.cpp buzzergroup_manager.cpp keyboard_buzzergroup.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

#target_link_libraries(${PROJECT_NAME} pthread boost_system)
