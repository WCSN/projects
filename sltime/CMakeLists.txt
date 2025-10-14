project(sltime)
cmake_minimum_required(VERSION 2.6)

include_directories(./src)
include_directories(/usr/include)
include_directories(/usr/local/include)
#include_directories(./src)

link_directories(/usr/local/lib)

aux_source_directory(src/ THUSAND_MAIN_SOURCE)
aux_source_directory(src/service THUSAND_SERVICE_SOURCE)

add_executable(${PROJECT_NAME} ${SOURCES} ${THUSAND_MAIN_SOURCE} ${THUSAND_SERVICE_SOURCE})
target_link_libraries(${PROJECT_NAME})

#if(NOT WITHOUT_TEST)
#
#   if(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)                                                                                       
#        target_link_libraries(${UNIT_TESTER} mt cppunit c)                                                                              
#    else(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)                                                                                     
#        target_link_libraries(${UNIT_TESTER} mt command cppunit dl)                                                                     
#    endif(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD) 
#
#	add_executable(${FUNC_TESTER} ${TEST_FUNC_SOURCES} ${TEST_FUNC_UTILS_SOURCES} ${TEST_MAIN})
#
#  if(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)                                                                                       
#        target_link_libraries(${FUNC_TESTER} mt cppunit c)                                                                              
#    else(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)                                                                                     
#        target_link_libraries(${FUNC_TESTER} mt command cppunit dl)                                                                     
#    endif(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD) 
#
#endif(NOT WITHOUT_TEST)

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
