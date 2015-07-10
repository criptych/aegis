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
            REQUIRE(g.calculateArea() == Approx(0.0));
        }
    }

    GIVEN("a single-point polygon") {
        g.points().push_back({0.0, 0.0});
        REQUIRE(g.points().size() == 1);

        THEN("the area is defined as zero") {
            REQUIRE(g.calculateArea() == Approx(0.0));
        }
    }

    GIVEN("a two-point polygon") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        REQUIRE(g.points().size() == 2);

        THEN("the area is defined as zero") {
            REQUIRE(g.calculateArea() == Approx(0.0));
        }
    }

    GIVEN("a unit right triangle") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        REQUIRE(g.points().size() == 3);

        THEN("the area is 1/2") {
            REQUIRE(g.calculateArea() == Approx(0.5));
        }
    }

    GIVEN("a unit square") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        g.points().push_back({0.0, 1.0});
        REQUIRE(g.points().size() == 4);

        THEN("the area is 1") {
            REQUIRE(g.calculateArea() == Approx(1.0));
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
            REQUIRE(g.calculateArea() == Approx(5.0));
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

        THEN("findIntersections() is false") {
            REQUIRE(!g.findIntersections());
        }
    }

    GIVEN("a unit square") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        g.points().push_back({0.0, 1.0});
        REQUIRE(g.points().size() == 4);

        THEN("findIntersections() is false") {
            REQUIRE(!g.findIntersections());
        }
    }

    GIVEN("a bowtie") {
        g.points().push_back({0.0, 0.0});
        g.points().push_back({0.0, 1.0});
        g.points().push_back({1.0, 0.0});
        g.points().push_back({1.0, 1.0});
        REQUIRE(g.points().size() == 4);

        THEN("findIntersections() is true") {
            REQUIRE(g.findIntersections());
        }
    }

}

////////////////////////////////////////////////////////////////////////////////

SCENARIO("calculate statistics", "[stats]") {
    aeStats st;

    GIVEN("an empty set of data") {
        THEN("the mean is undefined") {
            REQUIRE(std::isnan(st.mean()));
        }
        THEN("the population variance is undefined") {
            REQUIRE(std::isnan(st.variance(true)));
        }
        THEN("the population standard deviation is undefined") {
            REQUIRE(std::isnan(st.stdev(true)));
        }
        THEN("the skewness is undefined") {
            REQUIRE(std::isnan(st.skewness()));
        }
        THEN("the kurtosis is undefined") {
            REQUIRE(std::isnan(st.kurtosis()));
        }
    }

    GIVEN("a list of sample data: [2, 4, 4, 4, 5, 5, 7, 9]") {
        double data[] = { 2, 4, 4, 4, 5, 5, 7, 9 };
        st.update(std::begin(data), std::end(data));

        THEN("the mean is 5.0") {
            REQUIRE(st.mean() == Approx(5.0).epsilon(aeEpsilon));
        }
        THEN("the population variance is 4.0") {
            REQUIRE(st.variance(true) == Approx(4.0).epsilon(aeEpsilon));
        }
        THEN("the population standard deviation is 2.0") {
            REQUIRE(st.stdev(true) == Approx(2.0).epsilon(aeEpsilon));
        }
        THEN("the skewness is 0.61386567") {
            REQUIRE(st.skewness() == Approx(0.61386567).epsilon(aeEpsilon));
        }
        THEN("the kurtosis is 2.4336") {
            REQUIRE(st.kurtosis() == Approx(2.4336).epsilon(aeEpsilon));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

