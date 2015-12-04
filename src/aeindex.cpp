////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "aeindex.hpp"
#include "aeexcept.hpp"

//! @see <http://www.superliminal.com/sources/sources.htm>

////////////////////////////////////////////////////////////////////////////////

template <typename K, typename T>
std::vector<K> aeRtreeIndexT<K, T>::search(const aeExtentT<T> & /*extent*/) const {
    throw aeNotImplementedError();
}

template <typename K, typename T>
void aeRtreeIndexT<K, T>::insert(const K & /*key*/, const aeExtentT<T> & /*extent*/) {
    throw aeNotImplementedError();
}

template <typename K, typename T>
void aeRtreeIndexT<K, T>::remove(const K & /*key*/) {
    throw aeNotImplementedError();
}

////////////////////////////////////////////////////////////////////////////////

template class aeRtreeIndexT<void*, double>;
template class aeRtreeIndexT<int, double>;

template class aeRtreeIndexT<void*, float>;
template class aeRtreeIndexT<int, float>;

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

