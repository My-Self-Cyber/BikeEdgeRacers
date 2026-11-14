cmake_minimum_required(VERSION 3.22.1)

project("bike_edge_racers")

# Compile C++20 for high-performance physics & game loop
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bike_edge_racers SHARED
    native-lib.cpp
    GameEngine.cpp
    Motorcycle.cpp
    TrafficSystem.cpp
)

find_library(log-lib log)

target_link_libraries(bike_edge_racers
    ${log-lib}
    GLESv3
    EGL
    android
)
