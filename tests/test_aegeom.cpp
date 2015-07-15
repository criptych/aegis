////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"
#include "aegeom.hpp"

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("polygon area", "[aeGeometry][area]") {
    aeGeometry g = { aeGeometry::Polygon };

    SECTION("empty polygon") {
        REQUIRE(g.points().size() == 0);
        CHECK(g.calculateArea() == Approx(0.0));
    }

    SECTION("one-point polygon") {
        g.points().push_back({0.0, 0.0});

        REQUIRE(g.points().size() == 1);
        CHECK(g.calculateArea() == Approx(0.0));
    }

    SECTION("two-point polygon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});

        REQUIRE(g.points().size() == 2);
        CHECK(g.calculateArea() == Approx(0.0));
    }

    SECTION("unit right triangle") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});

        REQUIRE(g.points().size() == 3);
        CHECK(g.calculateArea() == Approx(0.5));
    }

    SECTION("unit square") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        g.points().push_back({0.0, 1.0});

        REQUIRE(g.points().size() == 4);
        CHECK(g.calculateArea() == Approx(1.0));
    }

    SECTION("irregular hexagon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({3.0, 0.0});
        g.points().push_back({3.0, 1.0});
        g.points().push_back({2.0, 2.0});
        g.points().push_back({1.0, 2.0});
        g.points().push_back({0.0, 1.0});

        REQUIRE(g.points().size() == 6);
        CHECK(g.calculateArea() == Approx(5.0));
    }
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("polygon extent", "[aeGeometry][extent]") {
    aeGeometry g = { aeGeometry::Polygon };

    SECTION("empty polygon") {
        REQUIRE(g.points().size() == 0);
        CHECK(g.extent().isEmpty());
        CHECK(std::isnan(g.extent().min.x));
        CHECK(std::isnan(g.extent().min.y));
        CHECK(std::isnan(g.extent().max.x));
        CHECK(std::isnan(g.extent().max.y));
    }

    SECTION("one-point polygon") {
        g.points().push_back({0.0, 0.0});

        REQUIRE(g.points().size() == 1);
        CHECK(g.extent().isEmpty());
        CHECK(g.extent().min.x == Approx(0.0));
        CHECK(g.extent().min.y == Approx(0.0));
        CHECK(g.extent().max.x == Approx(0.0));
        CHECK(g.extent().max.y == Approx(0.0));
    }

    SECTION("two-point polygon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});

        REQUIRE(g.points().size() == 2);
        CHECK(!g.extent().isEmpty());
        CHECK(g.extent().min.x == Approx(0.0));
        CHECK(g.extent().min.y == Approx(0.0));
        CHECK(g.extent().max.x == Approx(1.0));
        CHECK(g.extent().max.y == Approx(0.0));
    }

    SECTION("unit right triangle") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});

        REQUIRE(g.points().size() == 3);
        CHECK(!g.extent().isEmpty());
        CHECK(g.extent().min.x == Approx(0.0));
        CHECK(g.extent().min.y == Approx(0.0));
        CHECK(g.extent().max.x == Approx(1.0));
        CHECK(g.extent().max.y == Approx(1.0));
    }

    SECTION("unit square") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        g.points().push_back({0.0, 1.0});

        REQUIRE(g.points().size() == 4);
        CHECK(!g.extent().isEmpty());
        CHECK(g.extent().min.x == Approx(0.0));
        CHECK(g.extent().min.y == Approx(0.0));
        CHECK(g.extent().max.x == Approx(1.0));
        CHECK(g.extent().max.y == Approx(1.0));
    }

    SECTION("irregular hexagon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({3.0, 0.0});
        g.points().push_back({3.0, 1.0});
        g.points().push_back({2.0, 2.0});
        g.points().push_back({1.0, 2.0});
        g.points().push_back({0.0, 1.0});

        REQUIRE(g.points().size() == 6);
        CHECK(!g.extent().isEmpty());
        CHECK(g.extent().min.x == Approx(0.0));
        CHECK(g.extent().min.y == Approx(0.0));
        CHECK(g.extent().max.x == Approx(3.0));
        CHECK(g.extent().max.y == Approx(2.0));
    }
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("simple polygons", "[aeGeometry][simple]") {
    aeGeometry g = { aeGeometry::Polygon };

    SECTION("unit right triangle") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});

        REQUIRE(g.points().size() == 3);
        CHECK(!g.findIntersections());
    }

    SECTION("unit square") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        g.points().push_back({0.0, 1.0});

        REQUIRE(g.points().size() == 4);
        CHECK(!g.findIntersections());
    }

    SECTION("bowtie") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({0.0, 1.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});

        REQUIRE(g.points().size() == 4);
        CHECK(g.findIntersections());
    }
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

