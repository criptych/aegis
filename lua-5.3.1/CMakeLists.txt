################################################################################
##
################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(AEGIS C)

SET(CMAKE_BUILD_TYPE Release)

SET(LIBSRC
    src/lapi.c
    src/lauxlib.c
    src/lbaselib.c
    src/lbitlib.c
    src/lcode.c
    src/lcorolib.c
    src/lctype.c
    src/ldblib.c
    src/ldebug.c
    src/ldo.c
    src/ldump.c
    src/lfunc.c
    src/lgc.c
    src/linit.c
    src/liolib.c
    src/llex.c
    src/lmathlib.c
    src/lmem.c
    src/loadlib.c
    src/lobject.c
    src/lopcodes.c
    src/loslib.c
    src/lparser.c
    src/lstate.c
    src/lstring.c
    src/lstrlib.c
    src/ltable.c
    src/ltablib.c
    src/ltm.c
    src/lua.hpp
    src/lundump.c
    src/lutf8lib.c
    src/lvm.c
    src/lzio.c
)

SET(LUASRC
    src/lua.c
)

SET(LUACSRC
    src/luac.c
)

################################################################################

ADD_LIBRARY(liblua STATIC ${LIBSRC})
SET_TARGET_PROPERTIES(liblua PROPERTIES OUTPUT_NAME "lua")

ADD_EXECUTABLE(lua ${LUASRC})
TARGET_LINK_LIBRARIES(lua liblua)

ADD_EXECUTABLE(luac ${LUACSRC})
TARGET_LINK_LIBRARIES(luac liblua)

################################################################################
##  EOF
################################################################################

