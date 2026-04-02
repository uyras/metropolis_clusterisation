cmake_minimum_required(VERSION 3.13)

project(metropolis_clusterisation)

set (CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(metropolis_clusterisation main.cpp MagneticSystem.cpp misc.cpp)
target_link_libraries(metropolis_clusterisation PRIVATE Threads::Threads)
