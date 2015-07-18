////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "aeproj.hpp"
#include "aeexcept.hpp"

////////////////////////////////////////////////////////////////////////////////

template <typename T>
aePointT<T> aeProjectionT<T>::toWebMercator(const aePointT<T> &p) {
    static const double degsToRads = aePi / 180.0;
    static const double scaleFactor = 128.0 / aePi;
    /*
     * x = (128/pi)*(2**zoom)*(lon+pi);
     * y = (128/pi)*(2**zoom)*(pi-ln(tan(pi/4+lat/2)));
     */
    aePointT<T> q(p);
    q.x = scaleFactor * (aePi + degsToRads * p.x);
    q.y = scaleFactor * (aePi - std::log(std::tan(aePi / 4.0 + degsToRads * p.y / 2.0)));
    return q;
}

template <typename T>
aePointT<T> aeProjectionT<T>::fromWebMercator(const aePointT<T> &p) {
    throw aeNotImplementedError();
}

////////////////////////////////////////////////////////////////////////////////

template class aeProjectionT<double>;
template class aeProjectionT<float>;

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

