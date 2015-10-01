////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AERTREE_HPP__
#define __AERTREE_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include "aeextent.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <cinttypes>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

template <typename K, typename T=double>
class aeRtreeT {
public:
    aeRtreeT(
        float minFill = 0.3f,
        unsigned int capacity = 32
    ): mMinFill(minFill), mCapacity(capacity) {
    }

    /**
     * Returns a vector of elements whose extents intersect the given extent.
     */
    std::vector<K> search(const aeExtentT<T> &extent) const;

    /**
     * Returns a vector of elements whose extents contain the given point.
     */
    std::vector<K> search(const aePointT<T> &point) const {
        return search(aeExtentT<T>(point, point));
    }

    /**
     * Inserts an element with the given extent.
     */
    void insert(const K &key, const aeExtentT<T> &extent);

    /**
     * Inserts an element at the given point.
     */
    void insert(const K &key, const aePointT<T> &point) {
        return insert(key, aeExtentT<T>(point, point));
    }

    /**
     * Removes the given element.
     */
    void remove(const K &key);

    /**
     * Removes the given element.
     */
    template <typename KI>
    void remove(const KI &first, const KI &last) {
        for (KI i = first; i != last; i++) {
            remove(*i);
        }
    }

private:
    struct Page {
        Page(): mChildren() {}
        ~Page() {
            for (Page *child : mChildren) { delete child; }
        }
        std::vector<Page*> mChildren;
    };

private:
    float mMinFill;
    unsigned int mCapacity;
    Page mRoot;
};

////////////////////////////////////////////////////////////////////////////////

typedef aeRtreeT<void*, double> aeRtree;
typedef aeRtreeT<int32_t, double> aeRtreeInt32;
typedef aeRtreeT<uint32_t, double> aeRtreeUint32;
typedef aeRtreeT<ptrdiff_t, double> aeRtreeInt;

////////////////////////////////////////////////////////////////////////////////

#endif // __AERTREE_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

