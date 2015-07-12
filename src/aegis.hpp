////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AEGIS_HPP__
#define __AEGIS_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include "aeconst.hpp"
#include "aeexcept.hpp"
#include "aestats.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <vector>

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct aePointT {
    T x;
    T y;
    T z;
    T m;

    aePointT(): x(), y(), z(), m() {
    }

    aePointT(
        const T &x,
        const T &y
    ): x(x), y(y), z(), m() {
    }

    aePointT(
        const T &x,
        const T &y,
        const T &z
    ): x(x), y(y), z(z), m() {
    }

    aePointT(
        const T &x,
        const T &y,
        const T &z,
        const T &m
    ): x(x), y(y), z(z), m(m) {
    }

    bool equals(const aePointT<T> &rhs, const T &epsilon = T(aeEpsilon)) const;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool aePointT<T>::equals(const aePointT<T> &p, const T &e) const {
    return std::abs(p.x - x) <= e &&
           std::abs(p.y - y) <= e &&
           std::abs(p.z - z) <= e &&
           std::abs(p.m - m) <= e;
}

template <typename T>
bool operator == (const aePointT<T> &a, const aePointT<T> &b) {
    return a.equals(b);
}

template <typename T>
bool operator != (const aePointT<T> &a, const aePointT<T> &b) {
    return !a.equals(b);
}

template <typename T>
aePointT<T> operator + (const aePointT<T> &a) {
    return a;
}

template <typename T>
aePointT<T> operator - (const aePointT<T> &a) {
    return aePointT<T>(-a.x, -a.y, -a.z, -a.m);
}

template <typename T>
aePointT<T> operator + (const aePointT<T> &a, const aePointT<T> &b) {
    return aePointT<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.m + b.m);
}

template <typename T>
aePointT<T> operator - (const aePointT<T> &a, const aePointT<T> &b) {
    return aePointT<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.m - b.m);
}

template <typename T>
aePointT<T> operator * (const aePointT<T> &a, const T &b) {
    return aePointT<T>(a.x * b, a.y * b, a.z * b, a.m * b);
}

template <typename T>
aePointT<T> operator * (const T &a, const aePointT<T> &b) {
    return aePointT<T>(a * b.x, a * b.y, a * b.z, a * b.m);
}

template <typename T>
aePointT<T> operator / (const aePointT<T> &a, const T &b) {
    return aePointT<T>(a.x / b, a.y / b, a.z / b, a.m / b);
}

template <typename T>
T dot(const aePointT<T> &a, const aePointT<T> &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
T det(const aePointT<T> &a, const aePointT<T> &b)
{
    return a.x * b.y - a.y * b.x; // cross(a, b).z
}

template <typename T>
aePointT<T> cross(const aePointT<T> &a, const aePointT<T> &b)
{
    return aePointT<T>(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

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

    Type getType() const { return mType; }
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

template <typename T>
class aeProjectionT {

protected:
    static aePointT<T> toWebMercator(const aePointT<T> &p);
    static aePointT<T> fromWebMercator(const aePointT<T> &p);
};

////////////////////////////////////////////////////////////////////////////////

typedef aePointT<double> aePoint;
typedef aeExtentT<double> aeExtent;
typedef aeGeometryT<double> aeGeometry;
typedef aeProjectionT<double> aeProjection;

////////////////////////////////////////////////////////////////////////////////

#endif // __AEGIS_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

