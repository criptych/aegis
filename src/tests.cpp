////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "aegis.hpp"
#include "catch.hpp"

////////////////////////////////////////////////////////////////////////////////

SCENARIO("calculate area of simple polygons", "[area]") {
    aeGeometry g = { aeGeometry::Polygon };

    GIVEN("an empty polygon") {
        REQUIRE(g.points().size() == 0);

        THEN("the area is defined as zero") {
            REQUIRE(g.calculateArea() == 0.0);
        }
    }

    GIVEN("a single-point polygon") {
        g.points().push_back({0.0, 0.0});
        REQUIRE(g.points().size() == 1);

        THEN("the area is defined as zero") {
            REQUIRE(g.calculateArea() == 0.0);
        }
    }

    GIVEN("a two-point polygon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        REQUIRE(g.points().size() == 2);

        THEN("the area is defined as zero") {
            REQUIRE(g.calculateArea() == 0.0);
        }
    }

    GIVEN("a unit right triangle") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        REQUIRE(g.points().size() == 3);

        THEN("the area is 1/2") {
            REQUIRE(g.calculateArea() == 0.5);
        }
    }

    GIVEN("a unit square") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        g.points().push_back({0.0, 1.0});
        REQUIRE(g.points().size() == 4);

        THEN("the area is 1") {
            REQUIRE(g.calculateArea() == 1.0);
        }
    }

    GIVEN("an irregular hexagon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({3.0, 0.0});
        g.points().push_back({3.0, 1.0});
        g.points().push_back({2.0, 2.0});
        g.points().push_back({1.0, 2.0});
        g.points().push_back({0.0, 1.0});
        REQUIRE(g.points().size() == 6);

        THEN("the area is 5") {
            REQUIRE(g.calculateArea() == 5.0);
        }
    }
}

SCENARIO("check if polygon is simple", "[simple]") {
    aeGeometry g = { aeGeometry::Polygon };

    GIVEN("a unit right triangle") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        REQUIRE(g.points().size() == 3);

        THEN("isSimple() is true") {
            REQUIRE(g.isSimple());
        }
    }

    GIVEN("a unit square") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        g.points().push_back({0.0, 1.0});
        REQUIRE(g.points().size() == 4);

        THEN("isSimple() is true") {
            REQUIRE(g.isSimple());
        }
    }

    GIVEN("a bowtie") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({0.0, 1.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        REQUIRE(g.points().size() == 4);

        THEN("isSimple() is false") {
            REQUIRE(!g.isSimple());
        }
    }

}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

