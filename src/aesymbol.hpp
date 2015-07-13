////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AESYMBOL_HPP__
#define __AESYMBOL_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

struct aeColor {
    float r;
    float g;
    float b;
    float a;

    aeColor(): r(), g(), b(), a() {
    }

    aeColor(
        float x,
        float a = 1.0f
    ): r(x), g(x), b(x), a(a) {
    }

    aeColor(
        float r,
        float g,
        float b,
        float a = 1.0f
    ): r(r), g(g), b(b), a(a) {
    }

    static const aeColor Clear;
    static const aeColor Black;
    static const aeColor White;
};

////////////////////////////////////////////////////////////////////////////////

struct aeSymbol {
    enum Type {} type;
    aeColor color;
};

struct aeLineSymbol : public aeSymbol {
    float width;
    enum EndType {} endType;
    enum JoinType {} joinType;
};

struct aeMarkerSymbol : public aeSymbol {
    enum Shape {} shape;
    float size;
    float angle;
    aeLineSymbol outline;
};

struct aeFillSymbol : public aeSymbol {
    enum Pattern {} pattern;
    float interval;
    float angle;
    aeLineSymbol outline;
};

////////////////////////////////////////////////////////////////////////////////

#endif // __AESYMBOL_HPP__

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

