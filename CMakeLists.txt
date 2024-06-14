cmake_minimum_required(VERSION 3.22)
project(conflict_based_search_mine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIR})

add_executable(a_star_mine a_star_main.cpp
        a_star.hpp a_star.cpp
        util.hpp util.cpp)

target_link_libraries(a_star_mine ${Boost_LIBRARIES})

add_executable(cbs_mine cbs_main.cpp
        high_level.hpp high_level.cpp
        low_level.hpp low_level.cpp
        util.hpp util.cpp)

target_link_libraries(cbs_mine ${Boost_LIBRARIES})