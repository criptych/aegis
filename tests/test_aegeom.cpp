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
        CHECK(g.area() == Approx(0.0));
    }

    SECTION("one-point polygon") {
        g.points().push_back({0.0, 0.0});

        REQUIRE(g.points().size() == 1);
        CHECK(g.area() == Approx(0.0));
    }

    SECTION("two-point polygon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});

        REQUIRE(g.points().size() == 2);
        CHECK(g.area() == Approx(0.0));
    }

    SECTION("unit right triangle") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});

        REQUIRE(g.points().size() == 3);
        CHECK(g.area() == Approx(0.5));
    }

    SECTION("unit square") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        g.points().push_back({0.0, 1.0});

        REQUIRE(g.points().size() == 4);
        CHECK(g.area() == Approx(1.0));
    }

    SECTION("irregular hexagon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({3.0, 0.0});
        g.points().push_back({3.0, 1.0});
        g.points().push_back({2.0, 2.0});
        g.points().push_back({1.0, 2.0});
        g.points().push_back({0.0, 1.0});

        REQUIRE(g.points().size() == 6);
        CHECK(g.area() == Approx(5.0));
    }
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("polygon centroid", "[aeGeometry][centroid]") {
    aeGeometry g = { aeGeometry::Polygon };

    SECTION("empty polygon") {
        CAPTURE(g.area());
        CAPTURE(g.centroid().x);
        CAPTURE(g.centroid().y);

        REQUIRE(g.points().size() == 0);
        CHECK(std::isnan(g.centroid().x));
        CHECK(std::isnan(g.centroid().y));
    }

    SECTION("one-point polygon") {
        g.points().push_back({0.0, 0.0});

        CAPTURE(g.area());
        CAPTURE(g.centroid().x);
        CAPTURE(g.centroid().y);

        REQUIRE(g.points().size() == 1);
        CHECK(g.centroid().x == Approx(0.0));
        CHECK(g.centroid().y == Approx(0.0));
    }

    SECTION("two-point polygon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});

        CAPTURE(g.area());
        CAPTURE(g.centroid().x);
        CAPTURE(g.centroid().y);

        REQUIRE(g.points().size() == 2);
        CHECK(g.centroid().x == Approx(0.5));
        CHECK(g.centroid().y == Approx(0.0));
    }

    SECTION("unit right triangle") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});

        CAPTURE(g.area());
        CAPTURE(g.centroid().x);
        CAPTURE(g.centroid().y);

        REQUIRE(g.points().size() == 3);
        CHECK(g.centroid().x == Approx(0.6666666667));
        CHECK(g.centroid().y == Approx(0.3333333333));
    }

    SECTION("unit square") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        g.points().push_back({0.0, 1.0});

        CAPTURE(g.area());
        CAPTURE(g.centroid().x);
        CAPTURE(g.centroid().y);

        REQUIRE(g.points().size() == 4);
        CHECK(g.centroid().x == Approx(0.5));
        CHECK(g.centroid().y == Approx(0.5));
    }

    SECTION("irregular hexagon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({3.0, 0.0});
        g.points().push_back({3.0, 1.0});
        g.points().push_back({2.0, 2.0});
        g.points().push_back({1.0, 2.0});
        g.points().push_back({0.0, 1.0});

        CAPTURE(g.area());
        CAPTURE(g.centroid().x);
        CAPTURE(g.centroid().y);

        REQUIRE(g.points().size() == 6);
        CHECK(g.centroid().x == Approx( 3.0/ 2.0));
        CHECK(g.centroid().y == Approx(13.0/15.0));
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

