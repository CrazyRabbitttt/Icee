
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
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    ADD_DEFINITIONS(-DOS_LINUX)
else (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    ADD_DEFINITIONS(-DOS_MAC)
endif ()


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

# Build the net library
FILE(GLOB ICEE_HTTP_SOURCE RELATIVE ${CMAKE_SOURCE_DIR} "src/http/*.cc")
ADD_LIBRARY(icee_http ${ICEE_HTTP_SOURCE})
TARGET_LINK_LIBRARIES(icee_http Threads::Threads)
TARGET_COMPILE_OPTIONS(icee_http PRIVATE ${CMAKE_COMPILER_FLAG})
target_include_directories(
        icee_http
        PUBLIC ${ICEE_SERVER_SRC_INCLUDE_DIR}   #将目录暴露给 http
)

#Build the Http_server
ADD_EXECUTABLE(http_server ${ICEE_SERVER_SRC_DIR}/http/http_server.cc)
TARGET_LINK_LIBRARIES(http_server icee_core icee_http)
TARGET_COMPILE_OPTIONS(http_server PRIVATE ${CMAKE_COMPILER_FLAG})
TARGET_INCLUDE_DIRECTORIES(
        http_server
        PUBLIC ${ICEE_SERVER_SRC_INCLUDE_DIR}  #将目录暴露给 http_server
)


#
#########################################################
### Test (DocTest)
#########################################################
#
#include test test frame
include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://gitee.com/acking-you/doctest.git
        GIT_TAG v2.4.9
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(doctest)

# Test thread_pool
ADD_EXECUTABLE(thread_pool_test ${ICEE_SERVER_TEST_DIR}/core/threadPool_test.cc)
target_link_libraries(thread_pool_test doctest_with_main)

# Test buffer
ADD_EXECUTABLE(buffer_test ${ICEE_SERVER_TEST_DIR}/core/buffer_test.cc)
target_link_libraries(buffer_test doctest_with_main icee_core)

# Test cache
ADD_EXECUTABLE(cache_test ${ICEE_SERVER_TEST_DIR}/core/cache_test.cc)
target_link_libraries(cache_test doctest_with_main icee_core)

# Test poller
ADD_EXECUTABLE(poller_test ${ICEE_SERVER_TEST_DIR}/core/poller_test.cc)
target_link_libraries(poller_test doctest_with_main icee_core)

#Test socket
ADD_EXECUTABLE(socket_test ${ICEE_SERVER_TEST_DIR}/core/socket_test.cc)
target_link_libraries(socket_test doctest_with_main icee_core)

#Test accept
ADD_EXECUTABLE(acceptor_test ${ICEE_SERVER_TEST_DIR}/core/acceptor_test.cc)
target_link_libraries(acceptor_test doctest_with_main icee_core)

#Test connection
ADD_EXECUTABLE(connection_test ${ICEE_SERVER_TEST_DIR}/core/connection_test.cc)
target_link_libraries(connection_test doctest_with_main icee_core)

#Test net_address
ADD_EXECUTABLE(net_address_test ${ICEE_SERVER_TEST_DIR}/core/net_address_test.cc)
target_link_libraries(net_address_test doctest_with_main icee_core)

#Test Looper Test
ADD_EXECUTABLE(looper_test ${ICEE_SERVER_TEST_DIR}/core/looper_test.cc)
target_link_libraries(looper_test doctest_with_main icee_core)


# Test Request
ADD_EXECUTABLE(request_test ${ICEE_SERVER_TEST_DIR}/http/request_test.cc)
target_link_libraries(request_test doctest_with_main icee_http)

# Test Response
ADD_EXECUTABLE(response_test ${ICEE_SERVER_TEST_DIR}/http/responses_test.cc)
target_link_libraries(response_test doctest_with_main icee_http)



