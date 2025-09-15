cmake_minimum_required(VERSION 3.16)
project(mqtt_cpp_demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)

add_executable(publisher src/publisher.cpp)
target_include_directories(publisher PRIVATE ${MOSQUITTO_INCLUDE_DIRS})
target_link_libraries(publisher PRIVATE ${MOSQUITTO_LIBRARIES})

add_executable(subscriber src/subscriber.cpp)
target_include_directories(subscriber PRIVATE ${MOSQUITTO_INCLUDE_DIRS})
target_link_libraries(subscriber PRIVATE ${MOSQUITTO_LIBRARIES})
