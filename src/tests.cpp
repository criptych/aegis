////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"
#include "aegis.hpp"

////////////////////////////////////////////////////////////////////////////////

SCENARIO("calculate area of simple polygons") {
    aeGeometry g = { AE_POLYGON };

    GIVEN("an empty polygon") {
        THEN("the area is defined as zero") {
            REQUIRE(g.calculateArea() == 0.0);
        }
    }

    g.points().push_back({0.0, 0.0});

    GIVEN("a single-point polygon") {
        THEN("the area is defined as zero") {
            REQUIRE(g.calculateArea() == 0.0);
        }
    }

    g.points().push_back({1.0, 0.0});

    GIVEN("a two-point polygon") {
        THEN("the area is defined as zero") {
            REQUIRE(g.calculateArea() == 0.0);
        }
    }

    g.points().push_back({1.0, 1.0});

    GIVEN("a unit right triangle") {
        THEN("the area is 1/2") {
            REQUIRE(g.calculateArea() == 0.5);
        }
    }

    g.points().push_back({0.0, 1.0});

    GIVEN("a unit square") {
        THEN("the area is 1") {
            REQUIRE(g.calculateArea() == 1.0);
        }
    }

    g.points().clear();
    g.points().push_back({0.0, 0.0});
    g.points().push_back({3.0, 0.0});
    g.points().push_back({3.0, 1.0});
    g.points().push_back({2.0, 2.0});
    g.points().push_back({1.0, 2.0});
    g.points().push_back({0.0, 1.0});

    GIVEN("an irregular hexagon") {
        THEN("the area is 5") {
            REQUIRE(g.calculateArea() == 5.0);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

