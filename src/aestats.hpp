////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AESTATS_HPP__
#define __AESTATS_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include "aeconst.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <cmath>

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
        mMin = mMax = T();
        mM4 = mM3 = mM2 = mM1 = T();
    }

    N count() const {
        return mN;
    }

    aeStatsT<T, N> &update(const T &x) {
        if (mN == 0) {
            mMin = mMax = x;
        } else {
            if (mMin > x) { mMin = x; }
            if (mMax < x) { mMax = x; }
        }

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
        if (rhs.mN > 0) {
            if (mMin > rhs.mMin) { mMin = rhs.mMin; }
            if (mMax > rhs.mMax) { mMax = rhs.mMax; }
        }

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

    T min() const {
        return mMin;
    }

    T max() const {
        return mMax;
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
    T mMin;
    T mMax;
    T mM1;
    T mM2;
    T mM3;
    T mM4;
};

////////////////////////////////////////////////////////////////////////////////

typedef aeStatsT<double> aeStats;

////////////////////////////////////////////////////////////////////////////////

#endif // __AESTATS_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

