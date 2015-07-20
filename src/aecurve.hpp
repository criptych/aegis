////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AECURVE_HPP__
#define __AECURVE_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

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
    T ipow(const T &t, std::integral_constant<unsigned int, N>) {
        return t * ipow(t, std::integral_constant<unsigned int, N-1>());
    }

    template <typename T>
    T ipow(const T &t, std::integral_constant<unsigned int, 0>) {
        return T(1);
    }

    template <typename T, typename X, unsigned int N>
    T bezier(const X &x, std::integral_constant<unsigned int, N>()
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

    virtual aePointT<T> evaluate(const X &x) const;

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

    virtual aePointT<T> evaluate(const X &x) const;

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

