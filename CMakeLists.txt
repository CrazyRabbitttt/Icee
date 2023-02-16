
########################################################
# Set up
########################################################
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

PROJECT(Icee
        VERSION 2023.02
        LANGUAGES C CXX
        )

SET(THREADS_PREFER_PTHREAD_FLAG ON)
FIND_PACKAGE(Threads REQUIRED)


message(${PROJECT_SOURCE_DIR})

# Formatting utility search path
set(TURTLE_SERVER_BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")

########################################################
## Include
########################################################
SET(ICEE_SERVER_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
SET(ICEE_SERVER_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)

include_directories(${ICEE_SERVER_SRC_INCLUDE_DIR})
message(${ICEE_SERVER_SRC_INCLUDE_DIR})

########################################################
## Build
########################################################

#Build the icee library
FILE(GLOB ICEE_CORE_SOURCE RELATIVE ${CMAKE_SOURCE_DIR} "src/core/*.cc")
ADD_LIBRARY(icee_core ${ICEE_CORE_SOURCE})
TARGET_LINK_LIBRARIES(icee_core Threads::Threads)


#FILE(GLOB ICEE_HTTP_SOURCE RELATIVE ${CMAKE_SOURCE_DIR} "src/http/*.cc")
#ADD_LIBRARY(icee_http ${ICEE_HTTP_SOURCE})


