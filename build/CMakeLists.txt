cmake_minimum_required(VERSION 2.8)

project(NSW)

aux_source_directory(${PROJECT_BINARY_DIR}/../src SRC_LIST)

include_directories(${PROJECT_BINARY_DIR}/../inc)

SET(CMAKE_BUILD_TYPE "Debug")

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(JPEG ${SRC_LIST})

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
  target_link_libraries(NSW ${MATH_LIBRARY})
endif()