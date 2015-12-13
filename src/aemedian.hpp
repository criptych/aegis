////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AEMEDIAN_HPP_INCLUDE_GUARD
#define AEMEDIAN_HPP_INCLUDE_GUARD 1

////////////////////////////////////////////////////////////////////////////////

#include "aeconst.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <vector>

////////////////////////////////////////////////////////////////////////////////

//  @note Technically computes the "remedian" value, which approximates the
//  median for very large datasets.  This implementation uses a fixed base (B)
//  and increases the exponent as needed.

template <typename T, typename N=unsigned long long, unsigned int B=11>
class aeMedianT {
private:
    static constexpr T mNaN = T(aeNaN);

public:
    aeMedianT() {
        clear();
    }

    template <typename I>
    aeMedianT(const I &i, const I &j) {
        clear();
        update(i, j);
    }

    void clear() {
        mN = N();
        mData.clear();
        mData.push_back(Level());
    }

    N count() const {
        return mN;
    }

    aeMedianT<T, N, B> &update(const T &x) {
        unsigned int i = 0;
        T t(x);

        while (mData[i].insert(t)) {
            t = mData[i].median();
            mData[i++].clear();
            if (i >= mData.size())
            {
                mData.push_back(Level());
            }
        }

        ++mN;

        return *this;
    }

    template <typename I>
    void update(const I &i, const I &j) {
        for (I k(i); k != j; ++k) {
            update(*k);
        }
    }

    T median() const {
        return mData.back().median();
    }

private:
    struct Level {
        unsigned int count;
        T entries[B];

        Level(): count() {}

        void clear() {
            count = 0;
        }

        bool insert(const T &value) {
            if (count < B) {
                // @todo keep entries sorted
                unsigned i, j;
                for (i = 0; i < count; i++) {
                    if (entries[i] >= value) {
                        break;
                    }
                }
                for (j = count; j > i; j--) {
                    entries[j] = entries[j-1];
                }
                entries[i] = value;
                ++count;
            }
            return (count >= B);
        }

        T median() const {
            return (count > 0) ? entries[count/2] : mNaN;
        }
    };

    N mN;
    std::vector<Level> mData;
};

////////////////////////////////////////////////////////////////////////////////

typedef aeMedianT<double, unsigned long long> aeMedian;

////////////////////////////////////////////////////////////////////////////////

#endif // AEMEDIAN_HPP_INCLUDE_GUARD

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

