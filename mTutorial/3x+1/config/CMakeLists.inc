
cmake_minimum_required(VERSION 2.6)

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )
 	add_definitions(-Wall -O0)
elseif( CMAKE_BUILD_TYPE STREQUAL "RELEASE")
 	add_definitions(-Wall -O3)	
elseif( CMAKE_BUILD_TYPE STREQUAL "DEBUG" )
 	add_definitions(-Wall -O0)	
endif()

message("Set TYPE BUILD ${CMAKE_BUILD_TYPE}")

include_directories(${PROJECT_ROOT_PATH}/src)

link_directories(${PROJECT_ROOT_PATH}/lib)
link_directories(/usr/local/lib)

aux_source_directory(${PROJECT_ROOT_PATH}/src/ PROJECT_MAIN_SOURCE)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_executable(${PROJECT_NAME} ${SOURCES} ${PROJECT_MAIN_SOURCE})
target_link_libraries(${PROJECT_NAME})
