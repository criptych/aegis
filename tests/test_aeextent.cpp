////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"
#include "aeextent.hpp"

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("extent operations", "[aeExtent][extent]") {
    aeExtent e1 = { aePoint(0.0, 0.0), aePoint(1.0, 1.0) };
    aeExtent e2 = { aePoint(1.0, 1.0), aePoint(2.0, 2.0) };

    SECTION("check attributes") {
        CHECK((!e1.isEmpty()));
        CHECK((!e2.isEmpty()));
    }

    SECTION("check union") {
        aeExtent e = e1 | e2;
        CHECK((!e.isEmpty()));
        CHECK((e.min.x == Approx(0.0)));
        CHECK((e.min.y == Approx(0.0)));
        CHECK((e.max.x == Approx(2.0)));
        CHECK((e.max.y == Approx(2.0)));
    }

    SECTION("check empty intersection") {
        aeExtent e = e1 & e2;
        CHECK((e.isEmpty()));
        CHECK((e.min.x == Approx(1.0)));
        CHECK((e.min.y == Approx(1.0)));
        CHECK((e.max.x == Approx(1.0)));
        CHECK((e.max.y == Approx(1.0)));
    }

    SECTION("check non-empty intersection") {
        aeExtent e3 = { aePoint(0.5, 0.5), aePoint(1.5, 1.5) };
        aeExtent e = e1 & e3;
        CHECK((!e.isEmpty()));
        CHECK((e.min.x == Approx(0.5)));
        CHECK((e.min.y == Approx(0.5)));
        CHECK((e.max.x == Approx(1.0)));
        CHECK((e.max.y == Approx(1.0)));
    }
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

