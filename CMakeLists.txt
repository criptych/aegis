################################################################################
##
################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(AEGIS CXX)

SET(LIBSRCS
    src/aegis.cpp
    src/aeconst.cpp
    src/aeexcept.cpp
    src/aegeom.cpp
    src/aelayer.cpp
    src/aepoint.cpp
    src/aeproj.cpp
    src/aestats.cpp
    src/aesymbol.cpp
)

SET(TESTSRCS
    src/testmain.cpp
    src/tests.cpp
)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Werror")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -DNDEBUG -g -Wall")
ELSE()
    MESSAGE(WARNING
        "Please ensure your compiler supports C++11, or add flags to enable it."
    )
ENDIF()

################################################################################

FIND_PACKAGE(Lua51 REQUIRED QUIET)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${LUA_INCLUDE_DIR}
)

LINK_LIBRARIES(
    ${LUA_LIBRARIES}
)

ADD_LIBRARY(libaegis ${LIBSRCS})
SET_TARGET_PROPERTIES(libaegis PROPERTIES OUTPUT_NAME "aegis")

ADD_EXECUTABLE(aegis-test ${TESTSRCS})
TARGET_LINK_LIBRARIES(aegis-test libaegis)

ADD_CUSTOM_TARGET(check COMMAND aegis-test)

################################################################################
##  EOF
################################################################################

