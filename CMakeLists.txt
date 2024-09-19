cmake_minimum_required(VERSION 3.10)
project(PathFinder)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

add_executable(PathFinder
    src/main.cpp
    src/grid.cpp
    src/node.cpp
    src/graph.cpp
    src/astar.cpp
    src/button.cpp
    src/dijkstra.cpp
)
include_directories(PathFinder src/include)
target_link_libraries(PathFinder sfml-graphics)
target_link_libraries(PathFinder ${SFML_LIBRARIES})
