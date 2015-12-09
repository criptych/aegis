////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"
#include "aecurve.hpp"
#include "aeexcept.hpp"

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::ostream &operator << (std::ostream &os, const aePointT<T> &p) {
    return os << '<' << p.x << ',' << p.y << ',' << p.z << ',' << p.m << '>';
}

template <typename T>
void CheckPoint(const aePointT<T> &p, const aePointT<T> &q, T epsilon) {
}

TEST_CASE("bezier curve evaluation", "[aeCurve]") {
    static const double EPSILON = 0.0000000001;

    aeQuadraticBezierCurve quad;
    aeCubicBezierCurve cubic;

    SECTION("empty curve") {
        REQUIRE((cubic.controlPoints().size() == 0));
        CHECK_THROWS_AS(cubic.evaluate(0.5), aeInvalidStateError);
        CHECK_THROWS_AS(cubic[0.5], aeInvalidStateError);
    }

    SECTION("quadratic curve") {
        quad.controlPoints().push_back(aePoint(0.0, 0.0));
        quad.controlPoints().push_back(aePoint(0.5, 1.0));
        quad.controlPoints().push_back(aePoint(1.0, 0.0));

        REQUIRE((quad.controlPoints().size() == 3));

        GIVEN("x == 0.25") {
            aePoint p(quad.evaluate(0.25));
            CHECK((p.x == Approx(0.250).epsilon(EPSILON)));
            CHECK((p.y == Approx(0.375).epsilon(EPSILON)));
        }

        GIVEN("x == 0.5") {
            aePoint p(quad.evaluate(0.50));
            CHECK((p.x == Approx(0.500).epsilon(EPSILON)));
            CHECK((p.y == Approx(0.500).epsilon(EPSILON)));
        }

        GIVEN("x == 0.75") {
            aePoint p(quad.evaluate(0.75));
            CHECK((p.x == Approx(0.750).epsilon(EPSILON)));
            CHECK((p.y == Approx(0.375).epsilon(EPSILON)));
        }
    }

    SECTION("control points missing") {
        cubic.controlPoints().push_back(aePoint(0.0, 0.0));
        cubic.controlPoints().push_back(aePoint(0.5, 1.0));
        cubic.controlPoints().push_back(aePoint(1.0, 0.0));

        REQUIRE((cubic.controlPoints().size() == 3));
        CHECK_THROWS_AS(cubic.evaluate(0.5), aeInvalidStateError);
        CHECK_THROWS_AS(cubic[0.5], aeInvalidStateError);
    }

    SECTION("simple U-curve") {
        cubic.controlPoints().push_back(aePoint(0.0, 0.0));
        cubic.controlPoints().push_back(aePoint(0.0, 1.0));
        cubic.controlPoints().push_back(aePoint(1.0, 1.0));
        cubic.controlPoints().push_back(aePoint(1.0, 0.0));

        REQUIRE((cubic.controlPoints().size() == 4));

        GIVEN("x == 0.25") {
            aePoint p(cubic.evaluate(0.25));
            CHECK((p.x == Approx(0.15625).epsilon(EPSILON)));
            CHECK((p.y == Approx(0.56250).epsilon(EPSILON)));
        }

        GIVEN("x == 0.5") {
            aePoint p(cubic.evaluate(0.5));
            CHECK((p.x == Approx(0.50000).epsilon(EPSILON)));
            CHECK((p.y == Approx(0.75000).epsilon(EPSILON)));
        }

        GIVEN("x == 0.75") {
            aePoint p(cubic.evaluate(0.75));
            CHECK((p.x == Approx(0.84375).epsilon(EPSILON)));
            CHECK((p.y == Approx(0.56250).epsilon(EPSILON)));
        }
    }

    SECTION("simple S-curve") {
        cubic.controlPoints().push_back(aePoint(0.0, 0.0));
        cubic.controlPoints().push_back(aePoint(0.0, 1.0));
        cubic.controlPoints().push_back(aePoint(1.0, 0.0));
        cubic.controlPoints().push_back(aePoint(1.0, 1.0));

        REQUIRE((cubic.controlPoints().size() == 4));

        GIVEN("x == 0.25") {
            aePoint p(cubic.evaluate(0.25));
            CHECK((p.x == Approx(0.15625).epsilon(EPSILON)));
            CHECK((p.y == Approx(0.43750).epsilon(EPSILON)));
        }

        GIVEN("x == 0.5") {
            aePoint p(cubic.evaluate(0.5));
            CHECK((p.x == Approx(0.50000).epsilon(EPSILON)));
            CHECK((p.y == Approx(0.75000).epsilon(EPSILON)));
        }

        GIVEN("x == 0.75") {
            aePoint p(cubic.evaluate(0.75));
            CHECK((p.x == Approx(0.84375).epsilon(EPSILON)));
            CHECK((p.y == Approx(0.56250).epsilon(EPSILON)));
        }
    }
}

TEST_CASE("NURBS curve evaluation", "[aeCurve]") {
    aeCubicNurbsCurve curve;

    SECTION("empty curve") {
        REQUIRE((curve.controlPoints().size() == 0));
        CHECK_THROWS_AS(curve.evaluate(0.5), aeInvalidStateError);
        CHECK_THROWS_AS(curve[0.5], aeInvalidStateError);
    }

    SECTION("control points missing") {
        curve.controlPoints().push_back(aePoint(0.0, 0.0));
        curve.controlPoints().push_back(aePoint(0.5, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 0.0));

        REQUIRE((curve.controlPoints().size() == 3));
        CHECK_THROWS_AS(curve.evaluate(0.5), aeInvalidStateError);
        CHECK_THROWS_AS(curve[0.5], aeInvalidStateError);
    }

    SECTION("simple U-curve") {
        curve.controlPoints().push_back(aePoint(0.0, 0.0));
        curve.controlPoints().push_back(aePoint(0.0, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 0.0));

        aePoint p;
        REQUIRE((curve.controlPoints().size() == 4));
        REQUIRE_NOTHROW(p = curve.evaluate(0.5));
        CHECK((p.x == Approx(0.5)));
        CHECK((p.y == Approx(0.75)));
    }

    SECTION("simple S-curve") {
        curve.controlPoints().push_back(aePoint(0.0, 0.0));
        curve.controlPoints().push_back(aePoint(0.0, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 0.0));
        curve.controlPoints().push_back(aePoint(1.0, 1.0));

        aePoint p;
        REQUIRE((curve.controlPoints().size() == 4));
        REQUIRE_NOTHROW(p = curve.evaluate(0.5));
        CHECK((p.x == Approx(0.5)));
        CHECK((p.y == Approx(0.5)));
    }
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

