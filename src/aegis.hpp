////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AEGIS_HPP__
#define __AEGIS_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <limits>

////////////////////////////////////////////////////////////////////////////////

static const double aeEpsilon = std::numeric_limits<double>::epsilon();

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


class aeGeometry {
public:
    enum Type {
        Point,
        Multipoint,
        Polyline,
        Polygon,
    };

public:
    aeGeometry(Type type): mType(type) {}

    std::vector<aePoint> &points() { return mPoints; }
    const std::vector<aePoint> &points() const { return mPoints; }

    Type getType() const { return mType; }

    double calculateArea() const;

    bool isSimple() const;

    bool findIntersections(std::vector<aePoint> &intersections) const;
    bool findIntersections() const;

private:
    Type mType;
    std::vector<aePoint> mPoints;

    bool findIntersections(std::vector<aePoint> &intersections, bool abortOnFirst) const;
};

////////////////////////////////////////////////////////////////////////////////

#endif // __AEGIS_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

