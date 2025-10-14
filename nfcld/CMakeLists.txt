project(m.profile)
cmake_minimum_required(VERSION 2.6)
#set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-deprecated")

# Define build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# set up debug postfix
set(CMAKE_DEBUG_POSTFIX d)

if(NOT MTLIB_PATH)
	set(MTLIB_PATH ../)
endif(NOT MTLIB_PATH)

message(STATUS "mtlib path: ${MTLIB_PATH}")

if(NOT MYSQL_PATH)
	set(MYSQL_PATH /usr/local/mysql)
endif(NOT MYSQL_PATH)

message(STATUS "MySQL path: ${MYSQL_PATH}")

#set install path
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "Install path: ${CMAKE_CURRENT_SOURCE_DIR}")

#get version
execute_process(COMMAND chmod +x ../getversions.sh)
execute_process(COMMAND ../getversions.sh)
execute_process(COMMAND cat version.tag OUTPUT_VARIABLE VERSION)
message(STATUS "Version: ${VERSION}")

if(NOT WITHOUT_TEST)
	set(WITHOUT_TEST NO)
endif(NOT WITHOUT_TEST)

message(STATUS "Without tests: ${WITHOUT_TEST}")

include_directories(${MTLIB_PATH}/include)
include_directories(/usr/local/include/libxml2)
include_directories(/usr/include)
include_directories(/usr/local/include)

#include_directories(command/src/)
#include_directories(${MYSQL_PATH}/include)
include_directories(./)

link_directories(${MTLIB_PATH}/lib)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
#set(CMAKE_INSTALL_RPATH /usr/local/lib;${MTLIB_PATH}/lib)

#add_subdirectory(lib)
add_subdirectory(nfparser nfparser)

