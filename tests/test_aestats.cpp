////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"
#include "aestats.hpp"

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("statistics calculation", "[aeStatistics]") {
    static const double EPSILON = 0.0000000001;

    SECTION("empty data set") {
        aeStatistics st;

        REQUIRE(st.count() == 0);
        CHECK(std::isnan(st.min()));
        CHECK(std::isnan(st.max()));
        CHECK(std::isnan(st.mean()));
        CHECK(std::isnan(st.popVariance()));
        CHECK(std::isnan(st.popStdev()));
        CHECK(std::isnan(st.variance()));
        CHECK(std::isnan(st.stdev()));
        CHECK(std::isnan(st.skewness()));
        CHECK(std::isnan(st.kurtosis()));
    }

    SECTION("single value [1]") {
        aeStatistics st;
        st.update(1.0);

        REQUIRE(st.count() == 1);
        CHECK(st.min() == Approx(1.0));
        CHECK(st.max() == Approx(1.0));
        CHECK(st.mean() == Approx(1.0).epsilon(EPSILON));
        CHECK(std::isnan(st.popVariance()));
        CHECK(std::isnan(st.popStdev()));
        CHECK(std::isnan(st.variance()));
        CHECK(std::isnan(st.stdev()));
        CHECK(std::isnan(st.skewness()));
        CHECK(std::isnan(st.kurtosis()));
    }

    SECTION("two values [1 2]") {
        aeStatistics st;
        st.update(1.0);
        st.update(2.0);

        REQUIRE(st.count() == 2);
        CHECK(st.min() == Approx(1.0));
        CHECK(st.max() == Approx(2.0));
        CHECK(st.mean() == Approx(1.5).epsilon(EPSILON));
        CHECK(st.popVariance() == Approx(0.25).epsilon(EPSILON));
        CHECK(st.popStdev() == Approx(0.5).epsilon(EPSILON));
        CHECK(st.variance() == Approx(0.5).epsilon(EPSILON));
        CHECK(st.stdev() == Approx(0.7071067812).epsilon(EPSILON));
        CHECK(st.skewness() == Approx(0.0).epsilon(EPSILON));
        CHECK(st.kurtosis() == Approx(-2.0).epsilon(EPSILON));
    }

    SECTION("sample data: [2, 4, 4, 4, 5, 5, 7, 9]") {
        double data[] = { 2, 4, 4, 4, 5, 5, 7, 9 };
        aeStatistics st(std::begin(data), std::end(data));

        REQUIRE(st.count() == 8);
        CHECK(st.min() == Approx(2.0));
        CHECK(st.max() == Approx(9.0));
        CHECK(st.mean() == Approx(5.0).epsilon(EPSILON));
        CHECK(st.popVariance() == Approx(4.0).epsilon(EPSILON));
        CHECK(st.popStdev() == Approx(2.0).epsilon(EPSILON));
        CHECK(st.variance() == Approx(4.5714285714).epsilon(EPSILON));
        CHECK(st.stdev() == Approx(2.1380899352).epsilon(EPSILON));
        CHECK(st.skewness() == Approx(0.656250).epsilon(EPSILON));
        CHECK(st.kurtosis() == Approx(-0.218750).epsilon(EPSILON));
    }

    SECTION("sample data: [2, 4, 4, 4] and [5, 5, 7, 9]") {
        double data1[] = { 2, 4, 4, 4 };
        double data2[] = { 5, 5, 7, 9 };

        aeStatistics st1(std::begin(data1), std::end(data1));
        aeStatistics st2(std::begin(data2), std::end(data2));

        SECTION("list 1 statistics") {
            REQUIRE(st1.count() == 4);
            CHECK(st1.min() == Approx(2.0));
            CHECK(st1.max() == Approx(4.0));
            CHECK(st1.mean() == Approx(3.5).epsilon(EPSILON));
            CHECK(st1.popVariance() == Approx(0.75).epsilon(EPSILON));
            CHECK(st1.popStdev() == Approx(0.8660254038).epsilon(EPSILON));
            CHECK(st1.variance() == Approx(1.0).epsilon(EPSILON));
            CHECK(st1.stdev() == Approx(1.0).epsilon(EPSILON));
            CHECK(st1.skewness() == Approx(-1.1547005384).epsilon(EPSILON));
            CHECK(st1.kurtosis() == Approx(-0.6666666667).epsilon(EPSILON));
        }

        SECTION("list 2 statistics") {
            REQUIRE(st2.count() == 4);
            CHECK(st2.min() == Approx(5.0));
            CHECK(st2.max() == Approx(9.0));
            CHECK(st2.mean() == Approx(6.5).epsilon(EPSILON));
            CHECK(st2.popVariance() == Approx(2.75).epsilon(EPSILON));
            CHECK(st2.popStdev() == Approx(1.6583123952).epsilon(EPSILON));
            CHECK(st2.variance() == Approx(3.6666666667).epsilon(EPSILON));
            CHECK(st2.stdev() == Approx(1.9148542155).epsilon(EPSILON));
            CHECK(st2.skewness() == Approx(0.4933822002).epsilon(EPSILON));
            CHECK(st2.kurtosis() == Approx(-1.3719008264).epsilon(EPSILON));
        }

        aeStatistics st(st1 + st2);

        SECTION("combined statistics") {
            REQUIRE(st.count() == 8);
            CHECK(st.min() == Approx(2.0));
            CHECK(st.max() == Approx(9.0));
            CHECK(st.mean() == Approx(5.0).epsilon(EPSILON));
            CHECK(st.popVariance() == Approx(4.0).epsilon(EPSILON));
            CHECK(st.popStdev() == Approx(2.0).epsilon(EPSILON));
            CHECK(st.variance() == Approx(4.5714285714).epsilon(EPSILON));
            CHECK(st.stdev() == Approx(2.1380899352).epsilon(EPSILON));
            CHECK(st.skewness() == Approx(0.656250).epsilon(EPSILON));
            CHECK(st.kurtosis() == Approx(-0.218750).epsilon(EPSILON));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

