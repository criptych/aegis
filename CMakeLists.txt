################################################################################
##
################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(AEGIS CXX)

SET(AEGIS_BUILD_SHARED ON CACHE BOOL "Build SHARED library")
SET(AEGIS_BUILD_STATIC OFF CACHE BOOL "Build STATIC library")
SET(AEGIS_BUILD_TESTS ON CACHE BOOL "Build unit tests")
SET(AEGIS_BUILD_APP ON CACHE BOOL "Build application")

IF(NOT AEGIS_BUILD_SHARED AND NOT AEGIS_BUILD_STATIC)
    SET(AEGIS_BUILD_SHARED ON)
ENDIF()

SET(LIBSRCS
    src/aegis.hpp
    src/aeconst.hpp
    src/aecurve.hpp
    src/aeexcept.hpp
    src/aeextent.hpp
    src/aegeom.hpp
    src/aeindex.hpp
    src/aelayer.hpp
    src/aepoint.hpp
    src/aeproj.hpp
    src/aescript.hpp
    src/aestats.hpp
    src/aestream.hpp
    src/aesymbol.hpp
    src/aetypes.hpp
    src/aeuuid.hpp

    src/aegis.cpp
    src/aeconst.cpp
    src/aecurve.cpp
    src/aeexcept.cpp
    src/aeextent.cpp
    src/aegeom.cpp
    src/aeindex.cpp
    src/aelayer.cpp
    src/aepoint.cpp
    src/aeproj.cpp
    src/aescript.cpp
    src/aestats.cpp
    src/aestream.cpp
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
    tests/test_aeindex.cpp
    tests/test_aelayer.cpp
    tests/test_aepoint.cpp
    tests/test_aeproj.cpp
    tests/test_aescript.cpp
    tests/test_aestats.cpp
    tests/test_aestream.cpp
    tests/test_aesymbol.cpp
    tests/test_aetypes.cpp
    tests/test_aeuuid.cpp
)

SET(WX_COMPONENTS
    #propgrid adv net stc webview html xml
    qa aui richtext ribbon gl core base
)

SET(APPSRCS
    src/aemain.cpp
)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic -Werror")
ELSE()
    MESSAGE(WARNING
        "Please ensure your compiler supports C++11, or add flags to enable it."
    )
ENDIF()

################################################################################

ADD_SUBDIRECTORY(lua-5.3.1 EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(physfs-2.1 EXCLUDE_FROM_ALL)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/lua-5.3.1/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/physfs-2.1/src/
)

IF(AEGIS_BUILD_SHARED)
    ADD_LIBRARY(libshared SHARED ${LIBSRCS})
    SET_TARGET_PROPERTIES(libshared PROPERTIES OUTPUT_NAME "aegis")
    TARGET_LINK_LIBRARIES(libshared liblua physfs-static)
    SET(LIBNAME libshared)
ENDIF()

IF(AEGIS_BUILD_STATIC)
    ADD_LIBRARY(libstatic STATIC ${LIBSRCS})
    SET_TARGET_PROPERTIES(libstatic PROPERTIES OUTPUT_NAME "aegis")
    TARGET_LINK_LIBRARIES(libstatic liblua physfs-static)
    SET(LIBNAME libstatic)
ENDIF()

IF(AEGIS_BUILD_TESTS)
    ADD_EXECUTABLE(aegis-test ${TESTSRCS})
    TARGET_LINK_LIBRARIES(aegis-test ${LIBNAME})
ENDIF()

IF(AEGIS_BUILD_APP)
    FIND_PACKAGE(wxWidgets 3.0 COMPONENTS ${WX_COMPONENTS})
    IF(wxWidgets_FOUND)
        INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})
        ADD_EXECUTABLE(aegis ${APPSRCS})
        TARGET_LINK_LIBRARIES(aegis ${LIBNAME} ${wxWidgets_LIBRARIES})
    ENDIF()
ENDIF()

ENABLE_TESTING()
ADD_TEST(NAME run-test COMMAND aegis-test -s -d y -r xml -o testout.xml)

################################################################################
##  EOF
################################################################################

