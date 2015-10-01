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

#include "aestream.hpp"

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
        aeFileInputStream str(filename);
        char tmp[str.length()];
        str.read(tmp, sizeof(tmp));
        mSource.assign(tmp, sizeof(tmp));
        mName = "@"+filename;
        str.close();
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

#include "aeextent.hpp"
#include "aegeom.hpp"
#include "aelayer.hpp"
#include "aepoint.hpp"
#include "aeproj.hpp"
#include "aestats.hpp"
#include "aesymbol.hpp"
#include "aeuuid.hpp"

#include <map>

////////////////////////////////////////////////////////////////////////////////

class aeScriptBindingBase {
public:
    aeScriptBindingBase() {
    }

    ~aeScriptBindingBase() {
    }

    void addMethod(const std::string &name, lua_CFunction func) {
        mMethods[name] = func;
    }

protected:
    void bind(lua_State *state, const char *name) {
        luaL_newmetatable(state, name);
        lua_pushvalue(state, -1);
        lua_setfield(state, -2, "__index");
        for (const auto &method : mMethods) {
            lua_pushcfunction(state, method.second);
            lua_setfield(state, -2, method.first.c_str());
        }
        lua_newtable(state);
        lua_getfield(state, -2, "__new");
        lua_setfield(state, -2, "__call");
        lua_setmetatable(state, -2);
        lua_setfield(state, -2, name);
    }

    static void *create(lua_State *state, const char *type, size_t size) {
        void *p = lua_newuserdata(state, size);
        luaL_setmetatable(state, type);
        return p;
    }

    static void *test(lua_State *state, int index, const char *type) {
        return luaL_testudata(state, index, type);
    }

    static void *check(lua_State *state, int index, const char *type) {
        return luaL_checkudata(state, index, type);
    }

    static const char *toString(lua_State *state, int index, const char *type) {
        void *p = check(state, 1, type);
        return lua_pushfstring(state, "%s<@%p>", type, p);
    }

    std::map<std::string, lua_CFunction> mMethods;
};

template <typename T>
class aeScriptBinding : public aeScriptBindingBase {
public:
    static const char *Name;

public:
    aeScriptBinding() {
        mMethods["__new"] = __new;
        mMethods["__gc"] = __gc;
        mMethods["__tostring"] = __tostring;
    }

public:
    void bind(lua_State *state) {
        return aeScriptBindingBase::bind(state, Name);
    }

    static T *create(lua_State *state) {
        T *t = static_cast<T*>(aeScriptBindingBase::create(state, Name, sizeof(T)));
        new(t) T;
        return t;
    }

    static T *push(lua_State *state, const T &o) {
        T *t = static_cast<T*>(aeScriptBindingBase::create(state, Name, sizeof(T)));
        *t = o;
        return t;
    }

    static void destroy(lua_State *state, int index) {
        T *t = check(state, index);
        t->~T();
    }

    static T *test(lua_State *state, int index) {
        return static_cast<T*>(aeScriptBindingBase::test(state, index, Name));
    }

    static T *check(lua_State *state, int index) {
        return static_cast<T*>(aeScriptBindingBase::check(state, index, Name));
    }

    static int __new(lua_State *state) {
        create(state);
        return 1;
    }

    static int __gc(lua_State *state) {
        destroy(state, 1);
        return 0;
    }

    static int __tostring(lua_State *state) {
        aeScriptBindingBase::toString(state, 1, Name);
        return 1;
    }

};

////////////////////////////////////////////////////////////////////////////////

template <>
int aeScriptBinding< aeExtentT<lua_Number> >::__tostring(lua_State *state) {
    aeExtentT<lua_Number> *t = check(state, 1);
    lua_pushfstring(state, "Extent((%f,%f),(%f,%f))",
        t->min.x, t->min.y, t->max.x, t->max.y);
    return 1;
}

//~ template <>
//~ aeScriptBinding< aeStatsT<lua_Number> >::aeScriptBinding() {
    //~ mMethods["update"] = update;
//~ }

template <>
int aeScriptBinding<aeUuid>::__tostring(lua_State *state) {
    aeUuid *t = check(state, 1);
    std::string s = t->toString();
    lua_pushlstring(state, s.data(), s.size());
    return 1;
}

////////////////////////////////////////////////////////////////////////////////

#define BINDING(N,T) \
template <> const char *aeScriptBinding< T >::Name(#N); \
static aeScriptBinding< T > sBinding_##N;

#define BIND(state,N) (sBinding_##N.bind(state))

////////////////////////////////////////////////////////////////////////////////

BINDING(Extent, aeExtentT<lua_Number>);
BINDING(Layer, aeLayer);
BINDING(Point, aePointT<lua_Number>);
BINDING(Projection, aeProjectionT<lua_Number>);
BINDING(Stats, aeStatsT<lua_Number>);
BINDING(Symbol, aeSymbol);
BINDING(Uuid, aeUuid);

extern "C" int luaopen_aegis(lua_State *state) {
    lua_newtable(state);
    BIND(state, Extent);
    BIND(state, Layer);
    BIND(state, Point);
    BIND(state, Projection);
    BIND(state, Stats);
    BIND(state, Symbol);
    BIND(state, Uuid);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

