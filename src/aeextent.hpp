////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AEEXTENT_HPP__
#define __AEEXTENT_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include "aepoint.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <cmath>

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

    aeExtentT<T> &operator |= (const aePointT<T> &rhs) {
        if (min.x > rhs.x || std::isnan(min.x)) { min.x = rhs.x; }
        if (min.y > rhs.y || std::isnan(min.y)) { min.y = rhs.y; }
        if (min.z > rhs.z || std::isnan(min.z)) { min.z = rhs.z; }
        if (min.m > rhs.m || std::isnan(min.m)) { min.m = rhs.m; }
        if (max.x < rhs.x || std::isnan(max.x)) { max.x = rhs.x; }
        if (max.y < rhs.y || std::isnan(max.y)) { max.y = rhs.y; }
        if (max.z < rhs.z || std::isnan(max.z)) { max.z = rhs.z; }
        if (max.m < rhs.m || std::isnan(max.m)) { max.m = rhs.m; }
        return *this;
    }

    aeExtentT<T> &operator |= (const aeExtentT<T> &rhs) {
        if (min.x > rhs.min.x || std::isnan(min.x)) { min.x = rhs.min.x; }
        if (min.y > rhs.min.y || std::isnan(min.y)) { min.y = rhs.min.y; }
        if (min.z > rhs.min.z || std::isnan(min.z)) { min.z = rhs.min.z; }
        if (min.m > rhs.min.m || std::isnan(min.m)) { min.m = rhs.min.m; }
        if (max.x < rhs.max.x || std::isnan(max.x)) { max.x = rhs.max.x; }
        if (max.y < rhs.max.y || std::isnan(max.y)) { max.y = rhs.max.y; }
        if (max.z < rhs.max.z || std::isnan(max.z)) { max.z = rhs.max.z; }
        if (max.m < rhs.max.m || std::isnan(max.m)) { max.m = rhs.max.m; }
        return *this;
    }

    aeExtentT<T> &operator &= (const aeExtentT<T> &rhs) {
        if (min.x < rhs.min.x || std::isnan(min.x)) { min.x = rhs.min.x; }
        if (min.y < rhs.min.y || std::isnan(min.y)) { min.y = rhs.min.y; }
        if (min.z < rhs.min.z || std::isnan(min.z)) { min.z = rhs.min.z; }
        if (min.m < rhs.min.m || std::isnan(min.m)) { min.m = rhs.min.m; }
        if (max.x > rhs.max.x || std::isnan(max.x)) { max.x = rhs.max.x; }
        if (max.y > rhs.max.y || std::isnan(max.y)) { max.y = rhs.max.y; }
        if (max.z > rhs.max.z || std::isnan(max.z)) { max.z = rhs.max.z; }
        if (max.m > rhs.max.m || std::isnan(max.m)) { max.m = rhs.max.m; }
        return *this;
    }

    template <typename X>
    aeExtentT<T> operator | (const X &rhs) const {
        aeExtentT<T> result(*this);
        result |= rhs;
        return result;
    }

    template <typename X>
    aeExtentT<T> operator & (const X &rhs) const {
        aeExtentT<T> result(*this);
        result &= rhs;
        return result;
    }

    bool isEmpty() const {
        return (min.x == max.x && min.y == max.y) ||
                std::isnan(min.x) || std::isnan(min.y) ||
                std::isnan(max.x) || std::isnan(max.y);
    }
};

////////////////////////////////////////////////////////////////////////////////

typedef aeExtentT<double> aeExtent;

////////////////////////////////////////////////////////////////////////////////

#endif // __AEEXTENT_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

