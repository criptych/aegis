////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "aeproj.hpp"
#include "aeexcept.hpp"

////////////////////////////////////////////////////////////////////////////////

template <typename T>
aeProjectionT<T>::aeProjectionT() {
}

template <typename T>
aeProjectionT<T>::aeProjectionT(aeWKID wkid) {
}

template <typename T>
aePointT<T> aeProjectionT<T>::project(const aePointT<T> &p) const {
    switch (mWKID) {
        case aeWKID::WebMercator: {
            return toWebMercator(p);
        }

        //! @todo

        default: {
            return p;
        }
    }
}

template <typename T>
aePointT<T> aeProjectionT<T>::unproject(const aePointT<T> &p) const {
    switch (mWKID) {
        case aeWKID::WebMercator: {
            return fromWebMercator(p);
        }

        //! @todo

        default: {
            return p;
        }
    }
}

template <typename T>
aeWKID aeProjectionT<T>::getWKID() const {
    return mWKID;
}

template <typename T>
std::string aeProjectionT<T>::toString() const {
    //! @todo
    return std::string();
}

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

