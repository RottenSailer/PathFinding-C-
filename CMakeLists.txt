cmake_minimum_required(VERSION 3.10)
project(PathFinder)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

# Add all .cpp files from src directory
file(GLOB SOURCES "src/*.cpp")

add_executable(PathFinder ${SOURCES})

include_directories(PathFinder src/include)
include_directories(PathFinder src/include)
target_link_libraries(PathFinder sfml-graphics)
target_link_libraries(PathFinder ${SFML_LIBRARIES})
