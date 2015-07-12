////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AEGIS_HPP__
#define __AEGIS_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <exception>
#include <limits>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

static const double aeEpsilon = std::numeric_limits<double>::epsilon();
static const double aeNaN = std::numeric_limits<double>::quiet_NaN();
static const double aePi = 3.14159265358979323846;

////////////////////////////////////////////////////////////////////////////////

class aeExceptionBase : public std::exception {
public:
    aeExceptionBase(): mMessage() {
    }

    aeExceptionBase(
        const std::string &message
    ): mMessage(message) {
    }

    const char *what() const noexcept {
        return mMessage.c_str();
    }

private:
    std::string mMessage;
};

#define AE_EXCEPTION(B, T, P) \
class T : public B { \
public: \
    T(): B(P) {} \
    T(const std::string &message): B(std::string(P) + ": " + message) {} \
};

AE_EXCEPTION(aeExceptionBase, aeInternalError, "internal error");
AE_EXCEPTION(aeExceptionBase, aeNotImplementedError, "feature not implemented");

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

template <typename T, typename N=unsigned long long>
class aeStatsT {
private:
    static constexpr T mT2 = T(2), mT3 = T(3), mT4 = T(4), mT6 = T(6);

public:
    aeStatsT() {
        clear();
    }

    template <typename I>
    aeStatsT(const I &i, const I &j) {
        clear();
        update(i, j);
    }

    void clear() {
        mN = N();
        mM4 = mM3 = mM2 = mM1 = T();
    }

    N count() const {
        return mN;
    }

    aeStatsT<T, N> &update(const T &x) {
        T n1(mN);
        T n(++mN);
        T d(x - mM1);
        T dn(d / n);
        T dn2(dn * dn);
        T a(d * dn * n1);
        mM4 += a * dn2 * ((n - mT3) * n + mT3) + mT6 * dn2 * mM2 - mT4 * dn * mM3;
        mM3 += a * dn * (n - mT2) - mT3 * dn * mM2;
        mM2 += a;
        mM1 += dn;
        return *this;
    }

    aeStatsT<T, N> &update(const aeStatsT<T, N> &rhs) {
        T d(rhs.mM1 - mM1), d2(d * d), d3(d2 * d), d4(d2 * d2);
        T an(mN), bn(rhs.mN), an2(an * an), bn2(bn * bn), abn(an * bn);
        T n(an + bn), dn(an - bn), n2(n * n);
        mM4 += rhs.mM4 + d4 * abn * (an2 + bn2 - abn) / (n2 * n) +
               mT6 * d2 * (an2 * rhs.mM2 + bn2 * mM2) / n2 +
               mT4 * d * (an * rhs.mM3 - bn * mM3) / n;
        mM3 += rhs.mM3 + d3 * abn * dn / n2 +
               mT3 * d * (an * rhs.mM2 - bn * mM2) / n;
        mM2 += rhs.mM2 + d2 * T(mN) * T(rhs.mN) / n;
        mM1  = (mN * mM1 + rhs.mN * rhs.mM1) / n;
        mN += rhs.mN;
        return *this;
    }

    template <typename I>
    void update(const I &i, const I &j) {
        for (I k(i); k != j; ++k) {
            update(*k);
        }
    }

    T mean() const {
        return (mN > 0) ? mM1 : T(aeNaN);
    }

    T variance(bool finite = false) const {
        return (mN > 1) ? (mM2 / T(finite ? mN : (mN - 1))) : T(aeNaN);
    }

    T stdev(bool finite = false) const {
        return std::sqrt(variance(finite));
    }

    T skewness() const {
        return mM3 * std::sqrt(T(mN) / (mM2 * mM2 * mM2));
    }

    T kurtosis() const {
        return (T(mN) * mM4) / (mM2 * mM2) - mT3;
    }

    template <typename X>
    aeStatsT<T, N> &operator += (const X &rhs) {
        return update(rhs);
    }

    template <typename X>
    aeStatsT<T, N> operator + (const X &rhs) {
        aeStatsT t(*this);
        return t += rhs;
    }

private:
    N mN;
    T mM1;
    T mM2;
    T mM3;
    T mM4;
};

////////////////////////////////////////////////////////////////////////////////

typedef aePointT<double> aePoint;
typedef aeExtentT<double> aeExtent;
typedef aeGeometryT<double> aeGeometry;
typedef aeProjectionT<double> aeProjection;
typedef aeStatsT<double> aeStats;

////////////////////////////////////////////////////////////////////////////////

#endif // __AEGIS_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

