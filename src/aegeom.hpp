////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AEGEOM_HPP__
#define __AEGEOM_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include "aepoint.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct aeExtentT {
    aePointT<T> min;
    aePointT<T> max;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class aeGeometryT {
public:
    enum Type {
        Geometry,
        Point,
        LineString,
        Polygon,
        MultiPoint,
        MultiLineString,
        MultiPolygon,
        GeometryCollection,
        CircularString,
        CompoundCurve,
        CurvePolygon,
        MultiCurve,
        MultiSurface,
        Curve,
        Surface,
        PolyhedralSurface,
        TIN,
        Triangle,

        HasZ = 0x1000,
        HasM = 0x2000,
    };

    typedef std::vector< aePointT<T> > Points;

public:
    aeGeometryT(Type type): mType(type) {}

    Points &points() { return mPoints; }
    const Points &points() const { return mPoints; }

    Type type() const { return mType; }
    bool hasZ() const { return mType & HasZ; }
    bool hasM() const { return mType & HasM; }

    T calculateArea() const;

private:
    bool findIntersections(Points &intersections, bool abortOnFirst) const;

public:
    bool findIntersections() const {
        Points intersections;
        return findIntersections(intersections, true);
    }

    bool findIntersections(Points &intersections) const {
        return findIntersections(intersections, false);
    }

private:
    Type mType;
    Points mPoints;
};

////////////////////////////////////////////////////////////////////////////////

typedef aeExtentT<double> aeExtent;
typedef aeGeometryT<double> aeGeometry;

////////////////////////////////////////////////////////////////////////////////

#endif // __AEGEOM_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

