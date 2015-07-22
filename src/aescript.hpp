////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AESCRIPT_HPP__
#define __AESCRIPT_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include <string>

////////////////////////////////////////////////////////////////////////////////

class aeScript {
public:
    aeScript(const std::string &source, const std::string &name = "");

    std::string source() const;
    std::string &source();

    std::string name() const;

private:
    std::string mSource;
    std::string mName;
};

class aeScriptThread {
protected:
    aeScriptThread(struct lua_State *state);

public:
    aeScriptThread(const aeScriptThread &thread) = delete;
    aeScriptThread(aeScriptThread &&thread);

    ~aeScriptThread();

    aeScriptThread &loadSafeLibraries();
    aeScriptThread &loadBasicLibraries();
    aeScriptThread &loadAllLibraries();

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

#endif // __AESCRIPT_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

