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

    aeExtentT(
    ): min(aeNaN, aeNaN, aeNaN, aeNaN), max(aeNaN, aeNaN, aeNaN, aeNaN) {
    }

    aeExtentT(
        const aePointT<T> &min,
        const aePointT<T> &max
    ): min(min), max(max) {
    }

    bool isEmpty() const {
        return (min.x == max.x && min.y == max.y) ||
                std::isnan(min.x) || std::isnan(min.y) ||
                std::isnan(max.x) || std::isnan(max.y);
    }
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
    aeGeometryT(Type type): mType(type), mUpdateExtent(true) {}

    Points &points() { return mPoints; }
    const Points &points() const { return mPoints; }

private:
    void updateExtent() const;

public:
    const aeExtentT<T> &extent() const {
        if (mUpdateExtent) {
            updateExtent();
        }
        return mExtent;
    }

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
    mutable aeExtentT<T> mExtent;
    mutable bool mUpdateExtent;
};

////////////////////////////////////////////////////////////////////////////////

typedef aeExtentT<double> aeExtent;
typedef aeGeometryT<double> aeGeometry;

////////////////////////////////////////////////////////////////////////////////

#endif // __AEGEOM_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////
