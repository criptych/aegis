////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "aecurve.hpp"
#include "aeexcept.hpp"

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename X>
T lerp(const T &a, const T &b, const X &x) {
    return (X(1)-x) * a + x * b;
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Degree, typename T, typename X>
aePointT<T> aeBezierCurveT<Degree, T, X>::evaluate(const X &x) const {
    if (mControlPoints.size() <= Degree) {
        throw aeInvalidStateError(
            std::string("not enough control points (at least ") +
            std::to_string(Degree+1) + " required)"
        );
    }

    Points tempPoints(mControlPoints.begin(), mControlPoints.end());

    for (unsigned int d = Degree; d > 0; d--) {
        for (unsigned int i = 1; i <= d; i++) {
            tempPoints[i-1] = lerp(tempPoints[i-1], tempPoints[i], x);
        }
    }

    return tempPoints[0];
}

////////////////////////////////////////////////////////////////////////////////

template aePointT<double> aeBezierCurveT<2, double, double>::evaluate(
    const double &x
) const;

template aePointT<double> aeBezierCurveT<3, double, double>::evaluate(
    const double &x
) const;

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Degree, typename T, typename X>
aePointT<T> aeNurbsCurveT<Degree, T, X>::evaluate(const X &x) const {
    throw aeNotImplementedError();
}

////////////////////////////////////////////////////////////////////////////////

template aePointT<double> aeNurbsCurveT<2, double, double>::evaluate(
    const double &x
) const;

template aePointT<double> aeNurbsCurveT<3, double, double>::evaluate(
    const double &x
) const;

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

