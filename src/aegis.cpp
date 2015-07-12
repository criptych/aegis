////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}; // extern "C"

#include <algorithm>

#include "aegis.hpp"

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool aeGeometryT<T>::findIntersections(std::vector< aePointT<T> > &intersections, bool abortOnFirst) const {
    struct Segment {
        aePointT<T> a;
        aePointT<T> b;

        Segment(): a(), b() {}
        Segment(const aePointT<T> &a, const aePointT<T> &b): a(a), b(b) {}

        bool intersects(const Segment &s) const {
            aePointT<T> p;
            return intersects(s, p);
        }
        bool intersects(const Segment &s, aePointT<T> &p) const {
            aePointT<T> l1 = {   b.x -   a.x,   b.y -   a.y };
            aePointT<T> l2 = { s.b.x - s.a.x, s.b.y - s.a.y };

            T d = det(l1, l2);

            if (d != 0.0) {
                T t = det(l2, a - s.a) / d;

                if (t >= 0.0 && t <= 1.0) {
                    p = a + t * l1;
                    return true;
                }
            }

            return false;
        }
    };

    struct Event {
        enum Type { Invalid, LeftEnd, RightEnd, Intersection } type;
        aePointT<T> point;
        Segment *seg1;
        Segment *seg2;

        Event()
        {
        }

        Event(Type type):
            type(type), point(), seg1(), seg2()
        {
        }

        Event(Type type, const aePointT<T> &point):
            type(type), point(point), seg1(), seg2()
        {
        }

        Event(Type type, Segment *segment):
            type(type), point(), seg1(segment), seg2()
        {
        }

        bool operator == (const Event &e) const {
            return point == e.point;
        }

        bool operator < (const Event &e) const {
            return (point.x == e.point.x) ?
                (point.y < e.point.y) :
                (point.x < e.point.x);
        }
    };

    class SweepLine {
    public:
        void insert(const Segment *seg) {
            /// @todo
        }

        const Segment *above(const Segment *seg) const {
            /// @todo
            return nullptr;
        }

        const Segment *below(const Segment *seg) const {
            /// @todo
            return nullptr;
        }

    private:
    };

    class EventQueue {
    public:
        void push(const Event &e) {
            mEvents.push_back(e);
            std::push_heap(mEvents.begin(), mEvents.end());
        }

        Event peek() {
            return mEvents.front();
        }

        Event pop() {
            Event e = mEvents.front();
            std::pop_heap(mEvents.begin(), mEvents.end());
            mEvents.pop_back();
            return e;
        }

        bool contains(const Event &e) const {
            return std::find(mEvents.begin(), mEvents.end(), e) != mEvents.end();
        }

        bool empty() const {
            return mEvents.empty();
        }

    private:
        std::vector<Event> mEvents;
    };

/*
    Initialize event queue EQ = all segment endpoints;
    Sort EQ by increasing x and y;
    Initialize sweep line SL to be empty;
    Initialize output intersection list IL to be empty;
*/

    intersections.clear();

    SweepLine sweepLine;
    EventQueue eventQueue;

    unsigned int n = mPoints.size();

    std::vector<Segment> segments;
    segments.reserve(n);

    for (unsigned int i = 0, j = 1; i < n; ++i, ++j)
    {
        if (j >= n) {
            j = 0;
        }

        segments.push_back(Segment(mPoints[i], mPoints[j]));

        Event l(Event::LeftEnd, &segments.back());
        Event r(Event::RightEnd, &segments.back());

        if (mPoints[j].x < mPoints[i].x) {
            l.point = mPoints[j];
            r.point = mPoints[i];
        } else {
            l.point = mPoints[i];
            r.point = mPoints[j];
        }

        eventQueue.push(l);
        eventQueue.push(r);
    }

    while (!eventQueue.empty()) {
        Event e = eventQueue.pop();

        switch (e.type) {
            case Event::Invalid: {
                throw aeInternalError("aeGeometry::findIntersections: invalid Event type");
            }

            case Event::LeftEnd: {
                Segment *seg = e.seg1;
                sweepLine.insert(seg);
                aePointT<T> p;

                const Segment *above = sweepLine.above(seg);
                if (above && seg->intersects(*sweepLine.above(seg), p)) {
                    eventQueue.push(Event(Event::Intersection, p));
                }

                const Segment *below = sweepLine.below(seg);
                if (below && seg->intersects(*sweepLine.below(seg), p)) {
                    eventQueue.push(Event(Event::Intersection, p));
                }

                break;
            }

            case Event::RightEnd: {
                Segment *seg = e.seg1;
                sweepLine.insert(seg);
                aePointT<T> p;

                const Segment *above = sweepLine.above(seg);
                const Segment *below = sweepLine.below(seg);
                if (above && below && sweepLine.above(seg)->intersects(*sweepLine.below(seg), p)) {
                    Event e(Event::Intersection, p);
                    if (!eventQueue.contains(e)) {
                        eventQueue.push(e);
                    }
                }

                break;
            }

            case Event::Intersection: {
                intersections.push_back(e.point);
                //! @todo
                break;
            }
        }

/*
        if (E is a left endpoint) {
            Let segE = E's segment;
            Add segE to SL;
            Let segA = the segment Above segE in SL;
            Let segB = the segment Below segE in SL;
            if (I = Intersect( segE with segA) exists)
                Insert I into EQ;
            if (I = Intersect( segE with segB) exists)
                Insert I into EQ;
        }
        else if (E is a right endpoint) {
            Let segE = E's segment;
            Let segA = the segment Above segE in SL;
            Let segB = the segment Below segE in SL;
            Delete segE from SL;
            if (I = Intersect( segA with segB) exists)
                If (I is not in EQ already)
                    Insert I into EQ;
        }
        else {  // E is an intersection event
            Add E’s intersect point to the output list IL;
            Let segE1 above segE2 be E's intersecting segments in SL;
            Swap their positions so that segE2 is now above segE1;
            Let segA = the segment above segE2 in SL;
            Let segB = the segment below segE1 in SL;
            if (I = Intersect(segE2 with segA) exists)
                if (I is not in EQ already)
                    Insert I into EQ;
            if (I = Intersect(segE1 with segB) exists)
                if (I is not in EQ already)
                    Insert I into EQ;
        }
*/
    }

    throw aeNotImplementedError();
    return false;

    return intersections.size() > 0;
}

template <typename T>
T aeGeometryT<T>::calculateArea() const {
    unsigned int n = mPoints.size();
    T area = 0.0;

    if ((mType == Polygon) && (n >= 3)) {
        for (unsigned int i = 0, j = 1; i < n; ++i, ++j) {
            if (j >= n) {
                j = 0;
            }

            area += det(mPoints[i], mPoints[j]);
        }

        area *= 0.5;
    }

    return area;
}

template bool aeGeometry::findIntersections() const;
template double aeGeometry::calculateArea() const;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
aePointT<T> aeProjectionT<T>::toWebMercator(const aePointT<T> &p)
{
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
aePointT<T> aeProjectionT<T>::fromWebMercator(const aePointT<T> &p)
{
    throw aeNotImplementedError();
}

template aePoint aeProjection::toWebMercator(const aePoint &p);
template aePoint aeProjection::fromWebMercator(const aePoint &p);

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

