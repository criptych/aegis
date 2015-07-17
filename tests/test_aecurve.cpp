////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"
#include "aecurve.hpp"
#include "aeexcept.hpp"

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("bezier curve evaluation", "[aeCurve]") {
    aeCubicBezierCurve curve;

    SECTION("empty curve") {
        REQUIRE(curve.controlPoints().size() == 0);
        CHECK_THROWS_AS(curve.evaluate(0.5), aeInvalidStateError);
        CHECK_THROWS_AS(curve[0.5], aeInvalidStateError);
    }

    SECTION("control points missing") {
        curve.controlPoints().push_back(aePoint(0.0, 0.0));
        curve.controlPoints().push_back(aePoint(0.5, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 0.0));

        REQUIRE(curve.controlPoints().size() == 3);
        CHECK_THROWS_AS(curve.evaluate(0.5), aeInvalidStateError);
        CHECK_THROWS_AS(curve[0.5], aeInvalidStateError);
    }

    SECTION("simple U-curve") {
        curve.controlPoints().push_back(aePoint(0.0, 0.0));
        curve.controlPoints().push_back(aePoint(0.0, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 0.0));

        REQUIRE(curve.controlPoints().size() == 4);
        aePoint p = curve.evaluate(0.5);
        CHECK(p.x == Approx(0.5));
        CHECK(p.y == Approx(0.75));
    }

    SECTION("simple S-curve") {
        curve.controlPoints().push_back(aePoint(0.0, 0.0));
        curve.controlPoints().push_back(aePoint(0.0, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 0.0));
        curve.controlPoints().push_back(aePoint(1.0, 1.0));

        REQUIRE(curve.controlPoints().size() == 4);
        aePoint p = curve.evaluate(0.5);
        CHECK(p.x == Approx(0.5));
        CHECK(p.y == Approx(0.5));
    }
}

TEST_CASE("NURBS curve evaluation", "[aeCurve]") {
    aeCubicNurbsCurve curve;

    SECTION("empty curve") {
        REQUIRE(curve.controlPoints().size() == 0);
        CHECK_THROWS_AS(curve.evaluate(0.5), aeInvalidStateError);
        CHECK_THROWS_AS(curve[0.5], aeInvalidStateError);
    }

    SECTION("control points missing") {
        curve.controlPoints().push_back(aePoint(0.0, 0.0));
        curve.controlPoints().push_back(aePoint(0.5, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 0.0));

        REQUIRE(curve.controlPoints().size() == 3);
        CHECK_THROWS_AS(curve.evaluate(0.5), aeInvalidStateError);
        CHECK_THROWS_AS(curve[0.5], aeInvalidStateError);
    }

    SECTION("simple U-curve") {
        curve.controlPoints().push_back(aePoint(0.0, 0.0));
        curve.controlPoints().push_back(aePoint(0.0, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 0.0));

        REQUIRE(curve.controlPoints().size() == 4);
        aePoint p = curve.evaluate(0.5);
        CHECK(p.x == Approx(0.5));
        CHECK(p.y == Approx(0.75));
    }

    SECTION("simple S-curve") {
        curve.controlPoints().push_back(aePoint(0.0, 0.0));
        curve.controlPoints().push_back(aePoint(0.0, 1.0));
        curve.controlPoints().push_back(aePoint(1.0, 0.0));
        curve.controlPoints().push_back(aePoint(1.0, 1.0));

        REQUIRE(curve.controlPoints().size() == 4);
        aePoint p = curve.evaluate(0.5);
        CHECK(p.x == Approx(0.5));
        CHECK(p.y == Approx(0.5));
    }
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

