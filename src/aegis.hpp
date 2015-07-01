////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AEGIS_HPP__
#define __AEGIS_HPP__ 1

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

    bool findIntersections(std::vector<aePoint> &intersections, bool abortOnFirst) const;

private:
    aeGeoType mType;
    std::vector<aePoint> mPoints;
};

////////////////////////////////////////////////////////////////////////////////

#endif // __AEGIS_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

