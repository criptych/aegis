////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "aegis.hpp"
#include "catch.hpp"

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("polygon area", "[geometry][area]") {
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

TEST_CASE("simple polygons", "[geometry][simple]") {
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

TEST_CASE("statistics calculation", "[stats]") {
    static const double EPSILON = 0.0000000001;

    SECTION("empty data set") {
        aeStats st;

        REQUIRE(st.count() == 0);
        CHECK(std::isnan(st.mean()));
        CHECK(std::isnan(st.variance(true)));
        CHECK(std::isnan(st.stdev(true)));
        CHECK(std::isnan(st.variance(false)));
        CHECK(std::isnan(st.stdev(false)));
        CHECK(std::isnan(st.skewness()));
        CHECK(std::isnan(st.kurtosis()));
    }

    SECTION("sample data: [2, 4, 4, 4, 5, 5, 7, 9]") {
        double data[] = { 2, 4, 4, 4, 5, 5, 7, 9 };
        aeStats st(std::begin(data), std::end(data));

        REQUIRE(st.count() == 8);
        CHECK(st.mean() == Approx(5.0).epsilon(EPSILON));
        CHECK(st.variance(true) == Approx(4.0).epsilon(EPSILON));
        CHECK(st.stdev(true) == Approx(2.0).epsilon(EPSILON));
        CHECK(st.variance(false) == Approx(4.5714285714).epsilon(EPSILON));
        CHECK(st.stdev(false) == Approx(2.1380899352).epsilon(EPSILON));
        CHECK(st.skewness() == Approx(0.656250).epsilon(EPSILON));
        CHECK(st.kurtosis() == Approx(-0.218750).epsilon(EPSILON));
    }
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

