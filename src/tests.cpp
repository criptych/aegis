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

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("polygon extent", "[geometry][extent]") {
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

    SECTION("single value [1]") {
        aeStats st;
        st.update(1.0);

        REQUIRE(st.count() == 1);
        CHECK(st.mean() == Approx(1.0).epsilon(EPSILON));
        CHECK(std::isnan(st.variance(true)));
        CHECK(std::isnan(st.stdev(true)));
        CHECK(std::isnan(st.variance(false)));
        CHECK(std::isnan(st.stdev(false)));
        CHECK(std::isnan(st.skewness()));
        CHECK(std::isnan(st.kurtosis()));
    }

    SECTION("two values [1 2]") {
        aeStats st;
        st.update(1.0);
        st.update(2.0);

        REQUIRE(st.count() == 2);
        CHECK(st.mean() == Approx(1.5).epsilon(EPSILON));
        CHECK(st.variance(true) == Approx(0.25).epsilon(EPSILON));
        CHECK(st.stdev(true) == Approx(0.5).epsilon(EPSILON));
        CHECK(st.variance(false) == Approx(0.5).epsilon(EPSILON));
        CHECK(st.stdev(false) == Approx(0.7071067812).epsilon(EPSILON));
        CHECK(st.skewness() == Approx(0.0).epsilon(EPSILON));
        CHECK(st.kurtosis() == Approx(-2.0).epsilon(EPSILON));
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

    SECTION("sample data: [2, 4, 4, 4] and [5, 5, 7, 9]") {
        double data1[] = { 2, 4, 4, 4 };
        double data2[] = { 5, 5, 7, 9 };

        aeStats st1(std::begin(data1), std::end(data1));
        aeStats st2(std::begin(data2), std::end(data2));

        SECTION("list 1 statistics") {
            REQUIRE(st1.count() == 4);
            CHECK(st1.mean() == Approx(3.5).epsilon(EPSILON));
            CHECK(st1.variance(true) == Approx(0.75).epsilon(EPSILON));
            CHECK(st1.stdev(true) == Approx(0.8660254038).epsilon(EPSILON));
            CHECK(st1.variance(false) == Approx(1.0).epsilon(EPSILON));
            CHECK(st1.stdev(false) == Approx(1.0).epsilon(EPSILON));
            CHECK(st1.skewness() == Approx(-1.1547005384).epsilon(EPSILON));
            CHECK(st1.kurtosis() == Approx(-0.6666666667).epsilon(EPSILON));
        }

        SECTION("list 2 statistics") {
            REQUIRE(st2.count() == 4);
            CHECK(st2.mean() == Approx(6.5).epsilon(EPSILON));
            CHECK(st2.variance(true) == Approx(2.75).epsilon(EPSILON));
            CHECK(st2.stdev(true) == Approx(1.6583123952).epsilon(EPSILON));
            CHECK(st2.variance(false) == Approx(3.6666666667).epsilon(EPSILON));
            CHECK(st2.stdev(false) == Approx(1.9148542155).epsilon(EPSILON));
            CHECK(st2.skewness() == Approx(0.4933822002).epsilon(EPSILON));
            CHECK(st2.kurtosis() == Approx(-1.3719008264).epsilon(EPSILON));
        }

        aeStats st(st1 + st2);

        SECTION("combined statistics") {
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
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

