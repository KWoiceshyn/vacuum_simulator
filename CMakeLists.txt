cmake_minimum_required (VERSION 3.11)
set(CMAKE_CXX_STANDARD 14)
project(vacuum_simulator)

set(SOURCES
        src/vac_simulator.cpp
        src/occ_grid.cpp
        )

add_library(vac_sim ${SOURCES})
target_include_directories(vac_sim PUBLIC include)

add_executable(main src/main.cpp)
target_link_libraries(main vac_sim)



