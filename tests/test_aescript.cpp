////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"
#include "aescript.hpp"
#include "aeexcept.hpp"

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("script", "[aeScript]") {
    aeScriptHost host;

    SECTION("sandboxing") {
        aeScript script("print 'print function should not be defined'");
        CHECK_THROWS_AS(host.execute(script), aeInvalidStateError);
    }

    SECTION("hello world") {
        host.loadBasicLibraries();
        aeScript script("print 'Hello, world!'");
        CHECK_NOTHROW(host.execute(script));
    }

    SECTION("counter") {
        host.loadBasicLibraries();
        aeScript script("for i = 1, 10 do print(i) end");
        CHECK_NOTHROW(host.execute(script));
    }
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

