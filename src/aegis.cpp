////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}; // extern "C"

#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>

#include "aegis.hpp"

////////////////////////////////////////////////////////////////////////////////

int orient(const aePoint &p, const aePoint &q, const aePoint &r) {
    double f = (r.x - q.x) * (q.y - p.y) - (r.y - q.y) * (q.x - p.x);
    return (f < 0.0) ? -1 : (f > 0.0) : 1 : 0;
}



bool aeGeometry::findIntersections(std::vector<aePoint> &intersections, bool abortOnFirst) const {
    struct Segment {
        aePoint a;
        aePoint b;

        Segment(): a(), b() {}
        Segment(const aePoint &a, const aePoint &b): a(a), b(b) {}

        bool intersects(const Segment &s) const {
            return false;
        }
    };

    struct Event {
        aePoint p;
        Segment *s;
    };

/*
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
*/

    return false;
}

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

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

