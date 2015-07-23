################################################################################
##
################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(AEGIS CXX)

SET(AEGIS_SHAREDLIB ON CACHE BOOL "Build SHARED library")
SET(AEGIS_STATICLIB OFF CACHE BOOL "Build STATIC library")
SET(AEGIS_TESTS ON CACHE BOOL "Build unit tests")

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
    src/aescript.hpp
    src/aestats.hpp
    src/aesymbol.hpp
    src/aetypes.hpp
    src/aeuuid.hpp

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
    src/aescript.cpp
    src/aestats.cpp
    src/aesymbol.cpp
    src/aetypes.cpp
    src/aeuuid.cpp
)

SET(TESTSRCS
    include/catch.hpp

    tests/testmain.cpp
    tests/test_aeconst.cpp
    tests/test_aecurve.cpp
    tests/test_aeexcept.cpp
    tests/test_aeextent.cpp
    tests/test_aegeom.cpp
    tests/test_aelayer.cpp
    tests/test_aepoint.cpp
    tests/test_aeproj.cpp
    tests/test_aertree.cpp
    tests/test_aescript.cpp
    tests/test_aestats.cpp
    tests/test_aesymbol.cpp
    tests/test_aetypes.cpp
    tests/test_aeuuid.cpp
)

SET(APPSRCS
    src/aemain.cpp
)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
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
    TARGET_LINK_LIBRARIES(libshared ${LUA_LIBRARY})
    SET(LIBNAME libshared)
ENDIF()

IF(AEGIS_STATICLIB)
    ADD_LIBRARY(libstatic STATIC ${LIBSRCS})
    SET_TARGET_PROPERTIES(libstatic PROPERTIES OUTPUT_NAME "aegis")
    TARGET_LINK_LIBRARIES(libstatic ${LUA_LIBRARY})
    SET(LIBNAME libstatic)
ENDIF()

IF(AEGIS_TESTS)
    ADD_EXECUTABLE(aegis-test ${TESTSRCS})
    TARGET_LINK_LIBRARIES(aegis-test ${LIBNAME})
ENDIF()

ADD_EXECUTABLE(aegis ${APPSRCS})
TARGET_LINK_LIBRARIES(aegis ${LIBNAME})

ADD_CUSTOM_TARGET(check COMMAND aegis-test)

################################################################################
##  EOF
################################################################################

