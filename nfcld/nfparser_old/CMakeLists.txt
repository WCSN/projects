project(nfparser_old)
cmake_minimum_required(VERSION 2.6)

# Define build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# set up debug postfix
set(CMAKE_DEBUG_POSTFIX d)

if(NOT VRADUIS_PATH)
    set(VRADUIS_PATH ../../)
endif(NOT VRADUIS_PATH)

include_directories(${VRADUIS_PATH}/include)
include_directories(src)

link_directories(/home/wocson/projects/lib)

aux_source_directory(src/ SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} mt command vradius_client)

#if(NOT WITHOUT_TEST)
#	set(TEST_MAIN ../tests/TestsMain.cpp)
#	
#	set(UNIT_TESTER "${PROJECT_NAME}_unit")
#	set(FUNC_TESTER "${PROJECT_NAME}_func")
#
#	add_executable(${UNIT_TESTER} ${SOURCES} ${TEST_SOURCES} ${TEST_MAIN})
#	target_link_libraries(${UNIT_TESTER} mt command cppunit dl)
#
#	add_executable(${FUNC_TESTER} ${TEST_FUNC_SOURCES} ${TEST_FUNC_UTILS_SOURCES} ${TEST_MAIN})
#	target_link_libraries(${FUNC_TESTER} mt command cppunit dl)
#endif(NOT WITHOUT_TEST)

#install binary
install(TARGETS ${PROJECT_NAME} DESTINATION bin/)

#install includes
#install(DIRECTORY etc/ DESTINATION etc
#	PATTERN ".svn" EXCLUDE
#	PATTERN "*~" EXCLUDE
#	PATTERN "test_configs" EXCLUDE)
#
##add version tag to lib.
#install(CODE "execute_process(COMMAND mv ${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME} ${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}.${VERSION})")
#install(CODE "execute_process(COMMAND ln -sf ${PROJECT_NAME}.${VERSION} ${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME})")
