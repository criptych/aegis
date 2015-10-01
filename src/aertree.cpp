////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "aertree.hpp"
#include "aeexcept.hpp"

////////////////////////////////////////////////////////////////////////////////

template <typename K, typename T>
std::vector<K> aeRtreeT<K, T>::search(const aeExtentT<T> &extent) const {
    throw aeNotImplementedError();
}

template <typename K, typename T>
void aeRtreeT<K, T>::insert(const K &key, const aeExtentT<T> &extent) {
    throw aeNotImplementedError();
}

template <typename K, typename T>
void aeRtreeT<K, T>::remove(const K &key) {
    throw aeNotImplementedError();
}

////////////////////////////////////////////////////////////////////////////////

template class aeRtreeT<void*, double>;
template class aeRtreeT<int32_t, double>;
template class aeRtreeT<uint32_t, double>;
template class aeRtreeT<ptrdiff_t, double>;

template class aeRtreeT<void*, float>;
template class aeRtreeT<int32_t, float>;
template class aeRtreeT<uint32_t, float>;
template class aeRtreeT<ptrdiff_t, float>;

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

