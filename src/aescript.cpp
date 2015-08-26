////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "aescript.hpp"
#include "aeexcept.hpp"

////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}; // extern "C"

////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

aeScript::aeScript(
    const std::string &source
): mSource(source), mName(source) {
}

aeScript::aeScript(
    const std::string &source,
    const std::string &name
): mSource(source), mName(name) {
    if (mName[0] != '@' && mName[0] != '=') {
        mName = '=' + mName;
    }
}

void aeScript::load(const std::string &filename) {
    try {
        std::ifstream str;
        str.open(filename);
        if (str) {
            str >> mSource;
            mName = "@"+filename;
            str.close();
        } else {
            mName = mSource = "";
        }
    }
    catch (...) {
        mName = mSource = "";
        throw;
    }
}

std::string aeScript::source() const {
    return mSource;
}

std::string &aeScript::source() {
    return mSource;
}

std::string aeScript::name() const {
    return mName;
}

std::string &aeScript::name() {
    return mName;
}

////////////////////////////////////////////////////////////////////////////////

aeScriptThread::aeScriptThread(struct lua_State *state): mState(state) {
    lua_pushthread(mState);
    lua_rawsetp(mState, LUA_REGISTRYINDEX, this);
}

int aeScriptThread::pushThread(const aeScriptThread &thread) {
    return lua_rawgetp(mState, LUA_REGISTRYINDEX, &thread);
}

aeScriptThread::aeScriptThread(aeScriptThread &&thread): mState() {
    std::swap(mState, thread.mState);
    lua_pushthread(mState);
    lua_rawsetp(mState, LUA_REGISTRYINDEX, this);
    lua_pushthread(thread.mState);
    lua_rawsetp(thread.mState, LUA_REGISTRYINDEX, &thread);
}

aeScriptThread::~aeScriptThread() {
    lua_pushnil(mState);
    lua_rawsetp(mState, LUA_REGISTRYINDEX, this);

    if (lua_pushthread(mState)) {
        lua_close(mState);
        mState = nullptr;
    }
}

aeScriptThread &aeScriptThread::loadSafeLibraries() {
    luaL_requiref(mState, "string", luaopen_string, 1);
    luaL_requiref(mState, "utf8", luaopen_utf8, 1);
    luaL_requiref(mState, "table", luaopen_table, 1);
    luaL_requiref(mState, "math", luaopen_math, 1);
    return *this;
}

aeScriptThread &aeScriptThread::loadBasicLibraries() {
    loadSafeLibraries();
    luaL_requiref(mState, "_G", luaopen_base, 1);
    luaL_requiref(mState, "package", luaopen_package, 1);
    luaL_requiref(mState, "coroutine", luaopen_coroutine, 1);
    return *this;
}

aeScriptThread &aeScriptThread::loadAllLibraries() {
    loadBasicLibraries();
    luaL_requiref(mState, "io", luaopen_io, 1);
    luaL_requiref(mState, "os", luaopen_os, 1);
    luaL_requiref(mState, "debug", luaopen_debug, 1);
    return *this;
}

aeScriptThread &aeScriptThread::loadLibrary(const char *name, lua_CFunction loader) {
    luaL_requiref(mState, name, loader, 1);
    return *this;
}

aeScriptThread &aeScriptThread::execute(const aeScript &script) {
    if (luaL_loadbuffer(
        mState,
        script.source().c_str(),
        script.source().size(),
        script.name().c_str()
    )) {
        throw aeArgumentError(lua_tostring(mState, -1));
    }
    if (
        lua_pushlstring(mState, script.name().c_str(), script.name().size()),
        lua_pcall(mState, 1, 0, 0)
    ) {
        throw aeInvalidStateError(lua_tostring(mState, -1));
    }
    return *this;
}

aeScriptThread aeScriptThread::spawn(const aeScript &script) {
    return std::move(spawn().execute(script));
}

aeScriptThread aeScriptThread::spawn() {
    return aeScriptThread(lua_newthread(mState));
}

////////////////////////////////////////////////////////////////////////////////

aeScriptHost::aeScriptHost(): aeScriptThread(luaL_newstate()) {
}

////////////////////////////////////////////////////////////////////////////////
//  Bindings and library
////////////////////////////////////////////////////////////////////////////////

extern "C" int luaopen_aegis(lua_State *state) {
    lua_newtable(state);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

