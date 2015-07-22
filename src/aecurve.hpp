////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AECURVE_HPP__
#define __AECURVE_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include "aeexcept.hpp"
#include "aepoint.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <type_traits>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace {
    template <unsigned int N>
    constexpr unsigned int factorial() {
        return N * factorial<N-1>();
    }

    template <>
    constexpr unsigned int factorial<0>() {
        return 1;
    }

    template <unsigned int N, unsigned int M>
    constexpr unsigned int binomial() {
        return factorial<N>() / (factorial<M>() * factorial<N-M>());
    }

    template <typename T, unsigned int N>
    struct _ipow {
        T operator () (const T &t) {
            return t * _ipow<T, N-1>()(t);
        }
    };

    template <typename T>
    struct _ipow<T, 0> {
        T operator () (const T &t) {
            return T(1);
        }
    };

    template <typename T, typename X, unsigned int N, unsigned int I>
    struct _sum {
        T operator () (
            const X &x,
            const T *t
        ) {
            return t[I] * _ipow<X, binomial<N, I>()>()(x) +
                   _sum<T, X, N, I-1>()(x, t);
        }
    };

    template <typename T, typename X, unsigned int N>
    struct _sum<T, X, N, 0> {
        T operator () (const X &x, const T *t) {
            return t[0] * _ipow<X, binomial<N, 0>()>()(x);
        }
    };

    template <typename T, typename X, unsigned int N>
    T bezier(const X &x, const T *t, std::integral_constant<unsigned int, N>) {
        return _sum<T, X, N, N>()(x, t);
    }
};

////////////////////////////////////////////////////////////////////////////////

template <typename C, typename T, typename X=T>
class aeCurveBaseT {
protected:
    aeCurveBaseT() {}

public:
    virtual aePointT<T> evaluate(const X &x) const = 0;

    aePointT<T> operator[] (const X &x) const {
        return this->evaluate(x);
    }
};

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Degree, typename T, typename X=T>
class aeBezierCurveT : public aeCurveBaseT< aeBezierCurveT<Degree, T, X>, T, X > {
public:
    typedef std::vector< aePointT<T> > Points;

public:
    aeBezierCurveT() {
    }

    template <typename I>
    aeBezierCurveT(
        const I &a,
        const I &b
    ): mControlPoints(a, b) {
    }

    virtual aePointT<T> evaluate(const X &x) const {
        if (mControlPoints.size() <= Degree) {
            throw aeInvalidStateError(
                std::string("not enough control points (at least ") +
                std::to_string(Degree+1) + " required)"
            );
        }

        return bezier<aePointT<T>, X, Degree>(
            x, &mControlPoints[0], std::integral_constant<unsigned int, Degree>()
        );
    }

    Points &controlPoints() { return mControlPoints; }
    const Points &controlPoints() const { return mControlPoints; }

private:
    Points mControlPoints;
};

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Degree, typename T, typename X=T>
class aeNurbsCurveT : public aeCurveBaseT< aeNurbsCurveT<Degree, T, X>, T, X > {
public:
    typedef std::vector< aePointT<T> > Points;
    typedef std::vector<T> Knots;

public:
    aeNurbsCurveT() {
    }

    virtual aePointT<T> evaluate(const X &x) const {
        throw aeNotImplementedError();
    }

    Points &controlPoints() { return mControlPoints; }
    const Points &controlPoints() const { return mControlPoints; }

    Knots &knots() { return mKnots; }
    const Knots &knots() const { return mKnots; }

private:
    Points mControlPoints;
    std::vector<T> mKnots;
};

////////////////////////////////////////////////////////////////////////////////

typedef aeBezierCurveT<2, double> aeQuadraticBezierCurve;
typedef aeBezierCurveT<3, double> aeCubicBezierCurve;
typedef aeNurbsCurveT<2, double> aeQuadraticNurbsCurve;
typedef aeNurbsCurveT<3, double> aeCubicNurbsCurve;

////////////////////////////////////////////////////////////////////////////////

#endif // __AECURVE_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

