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

private:
    void updateExtent() const {
        mExtent.min = mExtent.max = aePointT<T>::Undefined;

        for (aePointT<T> &p : mPoints) {
            if (mExtent.min.x > p.x || std::isnan(mExtent.min.x)) {
                mExtent.min.x = p.x;
            }
            if (mExtent.min.y > p.y || std::isnan(mExtent.min.y)) {
                mExtent.min.y = p.y;
            }
            if (mExtent.min.z > p.z || std::isnan(mExtent.min.z)) {
                mExtent.min.z = p.z;
            }
            if (mExtent.min.m > p.m || std::isnan(mExtent.min.m)) {
                mExtent.min.m = p.m;
            }
            if (mExtent.max.x < p.x || std::isnan(mExtent.max.x)) {
                mExtent.max.x = p.x;
            }
            if (mExtent.max.y < p.y || std::isnan(mExtent.max.y)) {
                mExtent.max.y = p.y;
            }
            if (mExtent.max.z < p.z || std::isnan(mExtent.max.z)) {
                mExtent.max.z = p.z;
            }
            if (mExtent.max.m < p.m || std::isnan(mExtent.max.m)) {
                mExtent.max.m = p.m;
            }
        }

        mUpdateExtent = false;
    }

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
    aeExtentT<T> mExtent;
    bool mUpdateExtent;
};

////////////////////////////////////////////////////////////////////////////////

typedef aeExtentT<double> aeExtent;
typedef aeGeometryT<double> aeGeometry;

////////////////////////////////////////////////////////////////////////////////

#endif // __AEGEOM_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

