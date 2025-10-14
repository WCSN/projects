cmake_minimum_required(VERSION 3.20.0)

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )
 	add_definitions(-Wall -O0)
elseif( CMAKE_BUILD_TYPE STREQUAL "RELEASE")
 	add_definitions(-Wall -O3)	
elseif( CMAKE_BUILD_TYPE STREQUAL "DEBUG" )
 	add_definitions(-Wall -O0)	
endif()

include_directories(${PROJECT_ROOT_PATH}/src)

link_directories(${PROJECT_ROOT_PATH}/lib)
link_directories(/usr/local/lib)

aux_source_directory(${PROJECT_ROOT_PATH}/src/ PROJECT_MAIN_SOURCE)

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++20" COMPILER_SUPPORTS_CXX20)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
elseif(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++ support. Please use a different C++ compiler.")
endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

message("-- C++ set STD:${CMAKE_CXX_FLAGS}")
message("-- Set TYPE BUILD: ${CMAKE_BUILD_TYPE}")
add_executable(${PROJECT_NAME} ${SOURCES} ${PROJECT_MAIN_SOURCE})
target_link_libraries(${PROJECT_NAME})
