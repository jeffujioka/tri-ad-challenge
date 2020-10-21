cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME tri-ad-challenge)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

STRING(TOLOWER ${CMAKE_BUILD_TYPE} JMF_BUILD_TYPE_DIR)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/output/${JMF_BUILD_TYPE_DIR}")

# FIXME pre-compile it
add_subdirectory(externals/googletest)

# 3rd Party libraries include
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

add_subdirectory(dev)