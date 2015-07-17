################################################################################
##
################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(AEGIS CXX)

SET(AEGIS_SHAREDLIB ON CACHE BOOL "Build SHARED library")
SET(AEGIS_STATICLIB OFF CACHE BOOL "Build STATIC library")

IF(NOT AEGIS_SHAREDLIB AND NOT AEGIS_STATICLIB)
    SET(AEGIS_SHAREDLIB ON)
ENDIF()

SET(LIBSRCS
    src/aegis.hpp
    src/aeconst.hpp
    src/aecurve.hpp
    src/aeexcept.hpp
    src/aeextent.hpp
    src/aegeom.hpp
    src/aelayer.hpp
    src/aepoint.hpp
    src/aeproj.hpp
    src/aertree.hpp
    src/aestats.hpp
    src/aesymbol.hpp

    src/aegis.cpp
    src/aeconst.cpp
    src/aecurve.cpp
    src/aeexcept.cpp
    src/aeextent.cpp
    src/aegeom.cpp
    src/aelayer.cpp
    src/aepoint.cpp
    src/aeproj.cpp
    src/aertree.cpp
    src/aestats.cpp
    src/aesymbol.cpp
)

SET(TESTSRCS
    include/catch.hpp

    tests/testmain.cpp
    tests/test_aecurve.cpp
    tests/test_aeextent.cpp
    tests/test_aegeom.cpp
    tests/test_aestats.cpp
)

SET(APPSRCS
    src/aemain.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${LUA_INCLUDE_DIR}
)

IF(AEGIS_SHAREDLIB)
    ADD_LIBRARY(libshared SHARED ${LIBSRCS})
    SET_TARGET_PROPERTIES(libshared PROPERTIES OUTPUT_NAME "aegis")
ENDIF()

IF(AEGIS_STATICLIB)
    ADD_LIBRARY(libstatic STATIC ${LIBSRCS})
    SET_TARGET_PROPERTIES(libstatic PROPERTIES OUTPUT_NAME "aegis")
ENDIF()

ADD_EXECUTABLE(aegis-test ${TESTSRCS})
ADD_EXECUTABLE(aegis ${APPSRCS})
TARGET_LINK_LIBRARIES(aegis ${LUA_LIBRARIES})

IF(AEGIS_SHAREDLIB)
    TARGET_LINK_LIBRARIES(aegis-test libshared)
    TARGET_LINK_LIBRARIES(aegis libshared)
ELSEIF(AEGIS_STATICLIB)
    TARGET_LINK_LIBRARIES(aegis-test libstatic)
    TARGET_LINK_LIBRARIES(aegis libstatic)
ENDIF()

ADD_CUSTOM_TARGET(check COMMAND aegis-test)

################################################################################
##  EOF
################################################################################

