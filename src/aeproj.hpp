////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AEPROJ_HPP_INCLUDE_GUARD
#define AEPROJ_HPP_INCLUDE_GUARD 1

////////////////////////////////////////////////////////////////////////////////

#include "aepoint.hpp"
#include "aetypes.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <string>

////////////////////////////////////////////////////////////////////////////////

enum class aeWKID : int32_t {
    Unknown = -1,
    NullTransform = 0,
    WebMercator = 3857
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class aeProjectionT {
public:
    aeProjectionT();
    aeProjectionT(aeWKID wkid);

    aePointT<T> project(const aePointT<T> &p) const;
    aePointT<T> unproject(const aePointT<T> &p) const;

    aeWKID getWKID() const;
    std::string toString() const;

protected:
    static aePointT<T> toWebMercator(const aePointT<T> &p);
    static aePointT<T> fromWebMercator(const aePointT<T> &p);

private:
    aeWKID mWKID;
    class aeDatum *mDatum;
    class aeVerticalDatum *mVDatum;
};

////////////////////////////////////////////////////////////////////////////////

typedef aeProjectionT<double> aeProjection;

////////////////////////////////////////////////////////////////////////////////

#endif // AEPROJ_HPP_INCLUDE_GUARD

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

