cmake_minimum_required(VERSION 3.5)
project(TimeTable)

# Set C++ version
set(CMAKE_CXX_STANDARD 14)

#Turn on warnings
set(CMAKE_CXX_FLAGS "-g -O3 -Wall -Wextra -Werror")

#Set your directories
set(INCLUDE_DIR include)
set(SOURCE_DIR src)

# Include directories
include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE HEADER_FILES ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME})

