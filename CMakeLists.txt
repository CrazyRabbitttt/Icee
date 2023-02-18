
########################################################
# Set up
########################################################
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -std=c++17)

PROJECT(Icee
        VERSION 2023.02
        LANGUAGES C CXX
        )

# 检测操作系统，同时能够适应 Linux & MacOs
MESSAGE("The environment system : ${CMAKE_SYSTEM_NAME}")
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    ADD_DEFINITIONS(-DOS_LINUX)
else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    ADD_DEFINITIONS(-DOS_MAC)
endif()



SET(THREADS_PREFER_PTHREAD_FLAG ON)
FIND_PACKAGE(Threads REQUIRED)

# Formatting utility search path
set(TURTLE_SERVER_BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")

########################################################
## Include
########################################################
SET(ICEE_SERVER_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
SET(ICEE_SERVER_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
SET(ICEE_SERVER_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
SET(ICEE_SERVER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
SET(ICEE_SERVER_DEMO_DIR ${PROJECT_SOURCE_DIR}/demo)


include_directories(${ICEE_SERVER_SRC_INCLUDE_DIR})
message(${ICEE_SERVER_SRC_INCLUDE_DIR})

########################################################
## Build
########################################################
set(CMAKE_COMPILER_FLAG -O3 -Wall -Werror)


#Build the icee library
FILE(GLOB ICEE_CORE_SOURCE RELATIVE ${CMAKE_SOURCE_DIR} "src/core/*.cc")
ADD_LIBRARY(icee_core ${ICEE_CORE_SOURCE})
TARGET_LINK_LIBRARIES(icee_core Threads::Threads)
TARGET_COMPILE_OPTIONS(icee_core PRIVATE ${CMAKE_COMPILER_FLAG})
target_include_directories(
        icee_core
        PUBLIC ${ICEE_SERVER_SRC_INCLUDE_DIR}   #将目录暴露给网络库target
)

#
#########################################################
### Test (DocTest)
#########################################################
#
##fetch the doctest from github
#INCLUDE(FetchContent)
#FETCHCONTENT_DECLARE(
#        doctest
#        GIT_REPOSITORY https://github.com/doctest/doctest.git
#        GIT_TAG v2.4.9
#        GIT_SHALLOW TRUE
#)
#FetchContent_MakeAvailable(doctest)
























