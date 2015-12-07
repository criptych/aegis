////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AEINDEX_HPP_INCLUDE_GUARD
#define AEINDEX_HPP_INCLUDE_GUARD 1

////////////////////////////////////////////////////////////////////////////////

#include "aeextent.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <cinttypes>
#include <map>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

template <typename K, typename T=double>
class aeIndexBaseT {
protected:
    aeIndexBaseT() {}

public:
    typedef std::map<K, aeExtentT<T> > KeyMap;

    typename KeyMap::const_iterator begin() const { return mKeyMap.begin(); }
    typename KeyMap::const_iterator end() const { return mKeyMap.end(); }

public:
    /**
     * Returns a vector of elements whose extents intersect the given extent.
     */
    virtual std::vector<K> search(const aeExtentT<T> &extent) const = 0;

    /**
     * Returns a vector of elements whose extents contain the given point.
     */
    std::vector<K> search(const aePointT<T> &point) const {
        return search(aeExtentT<T>(point, point));
    }

    /**
     * Inserts an element with the given extent.
     */
    virtual void insert(const K &key, const aeExtentT<T> &extent) = 0;

    /**
     * Inserts an element at the given point.
     */
    void insert(const K &key, const aePointT<T> &point) {
        return insert(key, aeExtentT<T>(point, point));
    }

    /**
     * Removes the given element.
     */
    virtual void remove(const K &key) = 0;

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
    KeyMap mKeyMap;
};

////////////////////////////////////////////////////////////////////////////////

template <typename K, typename T=double>
class aeRtreeIndexT : public aeIndexBaseT<K, T> {
public:
    aeRtreeIndexT(
        float minFill = 0.3f,
        unsigned int capacity = 32
    ): mMinFill(minFill), mCapacity(capacity) {
    }

    aeRtreeIndexT(const aeIndexBaseT<K, T> &other) {
        for (std::pair<K, aeExtentT<T> > i : other) {
            insert(i.first, i.second);
        }
    }

    std::vector<K> search(const aeExtentT<T> &extent) const;
    void insert(const K &key, const aeExtentT<T> &extent);
    void remove(const K &key);

private:
    struct Page {
        Page(): mChildren(), mExtent(), mKey() {}
        Page(const aeExtentT<T> &extent, const K &key): mChildren(), mExtent(extent), mKey(key) {}

        void insert(const K &key, const aeExtentT<T> &extent);

        std::vector<Page*> mChildren;
        aeExtentT<T> mExtent;
        K mKey;
    };

private:
    float mMinFill;
    unsigned int mCapacity;
    Page mRoot;
};

////////////////////////////////////////////////////////////////////////////////

typedef aeRtreeIndexT<void*, double> aeRtree;
typedef aeRtreeIndexT<int, double> aeRtreeInt;

////////////////////////////////////////////////////////////////////////////////

#endif // AEINDEX_HPP_INCLUDE_GUARD

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

