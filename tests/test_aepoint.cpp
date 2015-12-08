////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"
#include "aepoint.hpp"

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::ostream &operator << (std::ostream &os, const aePointT<T> &p) {
    return os << '<' << p.x << ',' << p.y << ',' << p.z << ',' << p.m << '>';
}

TEST_CASE("point primitive", "[aePoint]") {
    aePoint a(0.0, 0.0);
    aePoint b(1.0, 1.0);
    aePoint c(1.0, 2.0);
    aePoint d(0.0, 1.0);
    aePoint e(0.0, 2.0);
    aePoint f(0.5, 0.0);

    aePoint x(1.0, 2.0, 3.0);
    aePoint y(0.0, 1.0, 2.0);
    aePoint z(1.0, -2.0, 1.0);

    SECTION("equality") {
        CHECK((a == a));
        CHECK((b == b));
        CHECK((a != b));
        CHECK((b != a));
        CHECK((a.equals(b, 1.0)));
        CHECK((b.equals(a, 1.0)));
    }

    SECTION("scalar operations") {
        CHECK((a + b == b));
        CHECK((a + c == c));
        CHECK((b + d == c));
        CHECK((c - d == b));
        CHECK((c - b == d));
        CHECK((d * 2.0 == e));
        CHECK((c / 2.0 == d + f));
    }

    SECTION("vector operations") {
        CHECK((dot(a, b) == 0.0));
        CHECK((dot(b, d) == 1.0));
        CHECK((dot(d, e) == 2.0));
        CHECK((dot(b, c) == 3.0));
        CHECK((dot(c, e) == 4.0));

        CHECK((det(b, c) == 1.0));
        CHECK((det(f, e) == 1.0));
        CHECK((det(c, e) == 2.0));

        CHECK((cross(x, y) == z));
    }
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

