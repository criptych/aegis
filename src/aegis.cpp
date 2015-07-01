extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}; // extern "C"


#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>


struct aePoint {
    double x;
    double y;
    double z;
    double m;

    aePoint():
        x(), y(), z(), m()
    {
    }

    aePoint(
        double x,
        double y
    ):
        x(x), y(y), z(), m()
    {
    }

    aePoint(
        double x,
        double y,
        double z
    ):
        x(x), y(y), z(z), m()
    {
    }

    aePoint(
        double x,
        double y,
        double z,
        double m
    ):
        x(x), y(y), z(z), m(m)
    {
    }

    double dot(const aePoint &p) const;
};

enum aeGeoType {
    AE_POINT,
    AE_MULTIPOINT,
    AE_LINE,
    AE_POLYGON,
};

class aeGeometry {
public:
    aeGeometry(aeGeoType type): mType(type) {}

    std::vector<aePoint> &points() { return mPoints; }
    const std::vector<aePoint> &points() const { return mPoints; }

    double calculateArea() const;

private:
    aeGeoType mType;
    std::vector<aePoint> mPoints;
};

/*
bool aeGeometry::findIntersections(std::vector<aePoint> &intersections, bool abortOnFirst) {
    struct Segment {
        aePoint a;
        aePoint b;

        Segment(): a(), b() {}
        Segment(const aePoint &a, const aePoint &b): a(a), b(b) {}

        bool intersects(const Segment &s) const {
        }
    };

    struct Event {
        aePoint p;
        Segment *s;
    };

    Initialize event queue EQ = all segment endpoints;
    Sort EQ by increasing x and y;
    Initialize sweep line SL to be empty;
    Initialize output intersection list IL to be empty;

    while (EQ is nonempty) {
        Let E = the next event from EQ;
        if (E is a left endpoint) {
            Let segE = E's segment;
            Add segE to SL;
            Let segA = the segment Above segE in SL;
            Let segB = the segment Below segE in SL;
            if (I = Intersect( segE with segA) exists) 
                Insert I into EQ;
            if (I = Intersect( segE with segB) exists) 
                Insert I into EQ;
        }
        else if (E is a right endpoint) {
            Let segE = E's segment;
            Let segA = the segment Above segE in SL;
            Let segB = the segment Below segE in SL;
            Delete segE from SL;
            if (I = Intersect( segA with segB) exists) 
                If (I is not in EQ already) 
                    Insert I into EQ;
        }
        else {  // E is an intersection event
            Add E’s intersect point to the output list IL;
            Let segE1 above segE2 be E's intersecting segments in SL;
            Swap their positions so that segE2 is now above segE1;
            Let segA = the segment above segE2 in SL;
            Let segB = the segment below segE1 in SL;
            if (I = Intersect(segE2 with segA) exists)
                if (I is not in EQ already) 
                    Insert I into EQ;
            if (I = Intersect(segE1 with segB) exists)
                if (I is not in EQ already) 
                    Insert I into EQ;
        }
        remove E from EQ;
    }
    return IL;
}
*/

double aeGeometry::calculateArea() const {
    if (mType != AE_POLYGON) {
        return 0.0;
    }

    unsigned int i, j, n = mPoints.size();

    if (n < 3) {
        return 0.0;
    }

    double area = 0.0;

    for (i = 0, j = 1; i < n; i++, j++) {
        if (j >= n) {
            j = 0;
        }
        area += mPoints[i].x * mPoints[j].y;
        area -= mPoints[i].y * mPoints[j].x;
    }

    return 0.5 * std::abs(area);
}




void test() {
    aeGeometry testGeo = { AE_POLYGON };
    testGeo.points().push_back({0.0, 0.0});
    std::cout << "area = " << testGeo.calculateArea() << "\n";
    assert(testGeo.calculateArea() == 0.0);
    testGeo.points().push_back({1.0, 0.0});
    std::cout << "area = " << testGeo.calculateArea() << "\n";
    assert(testGeo.calculateArea() == 0.0);
    testGeo.points().push_back({1.0, 1.0});
    std::cout << "area = " << testGeo.calculateArea() << "\n";
    assert(testGeo.calculateArea() == 0.5);
    testGeo.points().push_back({0.0, 1.0});
    std::cout << "area = " << testGeo.calculateArea() << "\n";
    assert(testGeo.calculateArea() == 1.0);

    testGeo.points().clear();
    testGeo.points().push_back({0.0, 0.0});
    testGeo.points().push_back({3.0, 0.0});
    testGeo.points().push_back({3.0, 1.0});
    testGeo.points().push_back({2.0, 2.0});
    testGeo.points().push_back({1.0, 2.0});
    testGeo.points().push_back({0.0, 1.0});
    std::cout << "area = " << testGeo.calculateArea() << "\n";
    assert(testGeo.calculateArea() == 5.0);
}




extern "C"
int main(int argc, char *argv[]) {
    test();
    return 0;
}
