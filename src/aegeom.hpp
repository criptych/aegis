////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AEGEOM_HPP_INCLUDE_GUARD
#define AEGEOM_HPP_INCLUDE_GUARD 1

////////////////////////////////////////////////////////////////////////////////

#include "aeextent.hpp"
#include "aepoint.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <vector>

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
    aeGeometryT(Type type): mType(type), mNeedsUpdate(true) {}

    Points &points() { return mPoints; }
    const Points &points() const { return mPoints; }

private:
    void update() const;

public:
    const T &area() const {
        if (mNeedsUpdate) {
            update();
        }
        return mArea;
    }

    const aePointT<T> &centroid() const {
        if (mNeedsUpdate) {
            update();
        }
        return mCentroid;
    }

    const aeExtentT<T> &extent() const {
        if (mNeedsUpdate) {
            update();
        }
        return mExtent;
    }

    Type type() const { return mType; }
    bool hasZ() const { return mType & HasZ; }
    bool hasM() const { return mType & HasM; }

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
    mutable T mArea;
    mutable aePointT<T> mCentroid;
    mutable aeExtentT<T> mExtent;
    mutable bool mNeedsUpdate;
};

////////////////////////////////////////////////////////////////////////////////

typedef aeGeometryT<double> aeGeometry;

////////////////////////////////////////////////////////////////////////////////

#endif // AEGEOM_HPP_INCLUDE_GUARD

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

