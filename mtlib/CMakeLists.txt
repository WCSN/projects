project(mt)
cmake_minimum_required(VERSION 2.6)
#set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated")

message(STATUS "System: " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION})
message(STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})

if(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)

endif(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD) 

include_directories(include)
include_directories(/usr/local/include/libxml2)
include_directories(/usr/local/include)
include_directories(/usr/include)

#Checking include
include(${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
include(${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)

if(WITH_MYSQL)
	message(STATUS "Compile mysql -- YES")

	if(NOT DEFINED MYSQL_PATH)
		set(MYSQL_PATH "/usr/local/mysql")
	endif(NOT DEFINED MYSQL_PATH)

	include_directories(${MYSQL_PATH}/include)
	check_include_file(${MYSQL_PATH}/include/mysql/mysql.h HAVE_MYSQL_H)
	check_include_file(${MYSQL_PATH}/include/mysql/errmsg.h HAVE_ERRMSG_H)

	set(MYSQL_CLIENT_LIB mysqlclient)
	check_library_exists(${MYSQL_CLIENT_LIB} mysql_real_query "${MYSQL_PATH}/lib/mysql;/usr/lib;/usr/local/lib" HAVE_MYSQL_LIB)
	if(NOT HAVE_MYSQL_H OR NOT HAVE_ERRMSG_H OR NOT HAVE_MYSQL_LIB)
		message(FATAL_ERROR "No mysql headers or mysqlclient library. MYSQL path is ${MYSQL_PATH}. Check it. You can exclude mysql from compilation by using the flag 'WITH_MYSQL=NO'")
	endif(NOT HAVE_MYSQL_H OR NOT HAVE_ERRMSG_H OR NOT HAVE_MYSQL_LIB)
	aux_source_directory(src/data/mysql MYSQL_SOURCES)
	aux_source_directory(src/data/mysql/tests MYSQL_TESTS_SOURCES)
	aux_source_directory(src/data/mysql/tests/func MYSQL_TESTS_FUNC_SOURCES)

	link_directories(${MYSQL_PATH}/lib/mysql)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
	set(CMAKE_INSTALL_RPATH ${MYSQL_PATH}/lib/mysql)

else(WITH_MYSQL)
	message(STATUS "Compile mysql -- NO. To compile mysql add parameter 'WITH_MYSQL=YES'.")
endif()


if(WITH_SMPP)
	message(STATUS "Compile smpp -- YES")
	aux_source_directory(src/proto/smpp SMPP_SOURCES)
	aux_source_directory(src/proto/smpp/server SMPP_SERVER_SOURCES)
	aux_source_directory(src/proto/smpp/client SMPP_CLIENT_SOURCES)
	aux_source_directory(src/proto/smpp/pdu SMPP_PDU_SOURCES)
	aux_source_directory(src/proto/smpp/common SMPP_COMMON_SOURCES)
	
	set(SMPP_SOURCES ${SMPP_SOURCES} ${SMPP_SERVER_SOURCES} ${SMPP_CLIENT_SOURCES} ${SMPP_COMMON_SOURCES} ${SMPP_PDU_SOURCES})

else(WITH_SMPP)
	message(STATUS "Compile smpp -- NO. To compile smpp add parameter 'WITH_SMPP=YES'.")
endif()

aux_source_directory(src/common COMMON_SOURCES)
aux_source_directory(src/common/tests COMMON_TESTS_SOURCES)
aux_source_directory(src/common/tests/func COMMON_TESTS_FUNC_SOURCES)
aux_source_directory(src/file FILE_SOURCES)
aux_source_directory(src/file/tests FILE_TESTS_SOURCES)
aux_source_directory(src/file/tests/func FILE_TESTS_FUNC_SOURCES)
aux_source_directory(src/utils UTILS_SOURCES)
aux_source_directory(src/utils/tests UTILS_TESTS_SOURCES)
aux_source_directory(src/utils/tests/func UTILS_TESTS_FUNC_SOURCES)
aux_source_directory(src/thread THREAD_SOURCES)
aux_source_directory(src/thread/tests THREAD_TESTS_SOURCES)
aux_source_directory(src/thread/tests/func THREAD_TESTS_FUNC_SOURCES)
aux_source_directory(src/logger LOGGER_SOURCES)
aux_source_directory(src/logger/tests LOGGER_TESTS_SOURCES)
aux_source_directory(src/logger/tests/func LOGGER_TESTS_FUNC_SOURCES)
aux_source_directory(src/network NETWORK_SOURCES)
aux_source_directory(src/network/tests NETWORK_TESTS_SOURCES)
aux_source_directory(src/network/tests/func NETWORK_TESTS_FUNC_SOURCES)
aux_source_directory(src/tools TOOLS_SOURCES)
aux_source_directory(src/tools/tests TOOLS_TESTS_SOURCES)
aux_source_directory(src/tools/tests/func TOOLS_TESTS_FUNC_SOURCES)


set(LIBS pthread xml2 ${MYSQL_CLIENT_LIB})
link_directories(/usr/local/lib)

#get version
execute_process(COMMAND chmod +x ../getversions.sh)
execute_process(COMMAND ../getversions.sh)
execute_process(COMMAND cat version.tag OUTPUT_VARIABLE VERSION)
message(STATUS "Version: ${VERSION}")

#targets
add_library(mt SHARED ${COMMON_SOURCES} ${FILE_SOURCES} ${UTILS_SOURCES} ${THREAD_SOURCES} ${LOGGER_SOURCES} ${NETWORK_SOURCES} ${MYSQL_SOURCES} ${SMPP_SOURCES} ${TOOLS_SOURCES}) 
target_link_libraries(mt ${LIBS})

if(NOT WITHOUT_TEST)

	if(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)
	    set(CPPUNIT_LIB cppunit c)
	else(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)
	    set(CPPUNIT_LIB cppunit dl)
	endif(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD) 

	set(TEST_MAIN src/tests/TestsMain.cpp)
        link_directories(/usr/local/lib)

	add_executable (mt_test ${COMMON_SOURCES} ${COMMON_TESTS_SOURCES} ${FILE_SOURCES} ${FILE_TESTS_SOURCES} ${UTILS_SOURCES} ${UTILS_TESTS_SOURCES} ${TEST_MAIN} ${THREAD_SOURCES} ${THREAD_TESTS_SOURCES} ${LOGGER_SOURCES} ${LOGGER_TESTS_SOURCES} ${NETWORK_SOURCES} ${NETWORK_TESTS_SOURCES} ${MYSQL_SOURCES} ${MYSQL_TESTS_SOURCES} ${TOOLS_SOURCES} ${TOOLS_TESTS_SOURCES})
	target_link_libraries(mt_test ${CPPUNIT_LIB} ${LIBS})

	message(STATUS "Compile functional tester -- YES")
	link_directories(${CMAKE_CURRENT_BINARY_DIR})
	add_executable (mt_test_func ${COMMON_TESTS_FUNC_SOURCES} ${FILE_TESTS_FUNC_SOURCES} ${UTILS_TESTS_FUNC_SOURCES} ${TEST_MAIN} ${THREAD_TESTS_FUNC_SOURCES} ${LOGGER_TESTS_FUNC_SOURCES} ${NETWORK_TESTS_FUNC_SOURCES} ${MYSQL_TESTS_FUNC_SOURCES} ${TOOLS_TESTS_FUNC_SOURCES})
	add_dependencies(mt_test_func mt)
	target_link_libraries(mt_test_func mt ${CPPUNIT_LIB})
endif(NOT WITHOUT_TEST)

#set install path
if(NOT INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX /usr/local)
else(NOT CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
endif()
message(STATUS "Install path: ${CMAKE_INSTALL_PREFIX}")

#install lib
install(TARGETS mt DESTINATION lib/)

#install includes
install(DIRECTORY include/ DESTINATION include
	PATTERN ".svn" EXCLUDE
	PATTERN "*~" EXCLUDE)

#add version tag to lib.
install(CODE "execute_process(COMMAND mv ${CMAKE_INSTALL_PREFIX}/lib/libmt.so ${CMAKE_INSTALL_PREFIX}/lib/libmt.so.${VERSION})")
install(CODE "execute_process(COMMAND ln -sf libmt.so.${VERSION} ${CMAKE_INSTALL_PREFIX}/lib/libmt.so)")

#generate docs
install(CODE "execute_process(COMMAND chmod +x ../generetedox.sh)")
install(CODE "execute_process(COMMAND ../generetedox.sh ../)")
