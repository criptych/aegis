////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AEGIS_HPP__
#define __AEGIS_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <limits>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

static const double aeEpsilon = std::numeric_limits<double>::epsilon();
static const double aePi = 3.14159265358979323846;

////////////////////////////////////////////////////////////////////////////////

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

    bool equals(const aePoint &rhs, const double epsilon = aeEpsilon) const;
};

bool operator == (const aePoint &a, const aePoint &b);
bool operator != (const aePoint &a, const aePoint &b);

aePoint operator + (const aePoint &a);
aePoint operator - (const aePoint &a);
aePoint operator + (const aePoint &a, const aePoint &b);
aePoint operator - (const aePoint &a, const aePoint &b);
aePoint operator * (const aePoint &a, double b);
aePoint operator * (double a, const aePoint &b);
aePoint operator / (const aePoint &a, double b);

double dot(const aePoint &a, const aePoint &b);
double det(const aePoint &a, const aePoint &b);
aePoint cross(const aePoint &a, const aePoint &b);

////////////////////////////////////////////////////////////////////////////////

struct aeExtent {
    aePoint min;
    aePoint max;
};

////////////////////////////////////////////////////////////////////////////////

class aeGeometry {
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

public:
    aeGeometry(Type type): mType(type) {}

    std::vector<aePoint> &points() { return mPoints; }
    const std::vector<aePoint> &points() const { return mPoints; }

    Type getType() const { return mType; }
    bool hasZ() const { return mType & HasZ; }
    bool hasM() const { return mType & HasM; }

    double calculateArea() const;

    bool findIntersections(std::vector<aePoint> &intersections) const;
    bool findIntersections() const;

private:
    Type mType;
    std::vector<aePoint> mPoints;

    bool findIntersections(std::vector<aePoint> &intersections, bool abortOnFirst) const;
};

////////////////////////////////////////////////////////////////////////////////

class aeProjection {

protected:
    static aePoint toWebMercator(const aePoint &p);
    static aePoint fromWebMercator(const aePoint &p);
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class aeStatsT {
private:
    static constexpr T mT2 = T(2), mT3 = T(3), mT4 = T(4), mT6 = T(6);

public:
    aeStatsT() {
        clear();
    }

    template <typename I>
    aeStatsT(const I &i, const I &j) {
        update(i, j);
    }

    void clear() {
        mN = 0;
        mM[3] = mM[2] = mM[1] = mM[0] = T();
    }

    void update(const T &x) {
        T n1(mN++), n(mN);
        T d = x - mM[0];
        T dn = d / n;
        T dn2 = dn * dn;
        T a = d * dn * n1;
        mM[3] += a*dn2*((n-mT3)*n+mT3) + mT6*dn2*mM[1] - mT4*dn*mM[2];
        mM[2] += a*dn*(n-mT2) - mT3*dn*mM[1];
        mM[1] += a;
        mM[0] += dn;
    }

    template <typename I>
    void update(const I &i, const I &j) {
        for (I k(i); k != j; k++) {
            update(*k);
        }
    }

    T mean() const {
        return mM[0];
    }

    T variance(bool finite = false) const {
        return mM[1] / T(finite ? mN : (mN - 1));
    }

    T stdev(bool finite = false) const {
        return std::sqrt(variance(finite));
    }

    T skewness() const {
        /// @todo
        return T();
    }

    T kurtosis() const {
        return (T(mN) * mM[3]) / (mM[1] * mM[1]) - mT3;
    }

private:
    unsigned long mN;
    T mM[4];
};

typedef aeStatsT<double> aeStats;

////////////////////////////////////////////////////////////////////////////////

#endif // __AEGIS_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

