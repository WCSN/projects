project(nfparser)
cmake_minimum_required(VERSION 2.6)

if(NOT VRADUIS_PATH)
    set(VRADUIS_PATH ../../)
endif(NOT VRADUIS_PATH)

include_directories(${VRADUIS_PATH}/include)
include_directories(./src)
include_directories(./src/service)

link_directories(/home/wocson/projects/lib)
link_directories(/usr/local/lib)

aux_source_directory(src/ NFPARSER_MAIN_SOURCE)
aux_source_directory(src/service NFPARSER_SERVICE_SOURCE)

aux_source_directory(src/tests TEST_SOURCES)
aux_source_directory(src/tests/func TEST_FUNC_SOURCES)
aux_source_directory(src/tests/func/utils TEST_FUNC_UTILS_SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES} ${NFPARSER_MAIN_SOURCE} ${NFPARSER_SERVICE_SOURCE})
target_link_libraries(${PROJECT_NAME} mt)
#command vradius_client)

if(NOT WITHOUT_TEST)
	set(TEST_MAIN ../tests/TestsMain.cpp)
	
	set(UNIT_TESTER "${PROJECT_NAME}_unit")
	set(FUNC_TESTER "${PROJECT_NAME}_func")

	add_executable(${UNIT_TESTER} ${SOURCES} ${TEST_SOURCES} ${TEST_MAIN})

   if(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)                                                                                       
        target_link_libraries(${UNIT_TESTER} mt cppunit c)                                                                              
    else(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)                                                                                     
        target_link_libraries(${UNIT_TESTER} mt command cppunit dl)                                                                     
    endif(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD) 

	add_executable(${FUNC_TESTER} ${TEST_FUNC_SOURCES} ${TEST_FUNC_UTILS_SOURCES} ${TEST_MAIN})


   if(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)                                                                                       
        target_link_libraries(${FUNC_TESTER} mt cppunit c)                                                                              
    else(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)                                                                                     
        target_link_libraries(${FUNC_TESTER} mt command cppunit dl)                                                                     
    endif(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD) 


endif(NOT WITHOUT_TEST)

#install binary
#install(TARGETS ${PROJECT_NAME} DESTINATION bin/)

#install includes
#install(DIRECTORY etc/ DESTINATION etc
#	PATTERN ".svn" EXCLUDE
#	PATTERN "*~" EXCLUDE
#	PATTERN "test_configs" EXCLUDE)
#
##add version tag to lib.
#install(CODE "execute_process(COMMAND mv ${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME} ${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}.${VERSION})")
#install(CODE "execute_process(COMMAND ln -sf ${PROJECT_NAME}.${VERSION} ${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME})")
