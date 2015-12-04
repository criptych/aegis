////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AESCRIPT_HPP_INCLUDE_GUARD
#define AESCRIPT_HPP_INCLUDE_GUARD 1

////////////////////////////////////////////////////////////////////////////////

#include <string>

////////////////////////////////////////////////////////////////////////////////

class aeScript {
public:
    aeScript() {}
    aeScript(const std::string &source);
    aeScript(const std::string &source, const std::string &name);

    void load(const std::string &filename);

    std::string source() const;
    std::string &source();

    std::string name() const;
    std::string &name();

private:
    std::string mSource;
    std::string mName;
};

class aeScriptThread {
protected:
    aeScriptThread(struct lua_State *state);

    int pushThread(const aeScriptThread &thread);
    int pushThread() { return pushThread(*this); }

public:
    aeScriptThread(const aeScriptThread &thread) = delete;
    aeScriptThread(aeScriptThread &&thread);

    ~aeScriptThread();

    /**
     * Loads built-in libraries that are deemed "safe", with no facilities to
     * break sandboxing (under normal circumstances; bugs and malicious code
     * notwithstanding).
     */
    aeScriptThread &loadSafeLibraries();

    /**
     * Loads potentially unsafe built-in libraries, including facilities to
     * load external code or cause the interpreter to hang/freeze.
     */
    aeScriptThread &loadBasicLibraries();

    /**
     * Loads patently unsafe built-in libraries, including facilities to access
     * the filesystem, start external processes, close the application
     * unexpectedly, or directly manipulate the interpreter state.
     */
    aeScriptThread &loadAllLibraries();

    aeScriptThread &loadLibrary(const char *name, int(*loader)(struct lua_State*));

    aeScriptThread &execute(const aeScript &script);
    aeScriptThread spawn(const aeScript &script);
    aeScriptThread spawn();

private:
    struct lua_State *mState;
};

class aeScriptHost : public aeScriptThread {
public:
    aeScriptHost();
};

////////////////////////////////////////////////////////////////////////////////

extern "C" int luaopen_aegis(struct lua_State*);

////////////////////////////////////////////////////////////////////////////////

#endif // AESCRIPT_HPP_INCLUDE_GUARD

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

