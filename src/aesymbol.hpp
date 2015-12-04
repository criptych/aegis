////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AESYMBOL_HPP_INCLUDE_GUARD
#define AESYMBOL_HPP_INCLUDE_GUARD 1

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

    static const aeColor None;
    static const aeColor Black;
    static const aeColor White;
};

////////////////////////////////////////////////////////////////////////////////

struct aeSymbol {
    enum class Type {
        Marker,
        Line,
        Fill,
        Text
    } type;

    aeColor color;
};

struct aeLineSymbol : public aeSymbol {
    float width;

    enum class Style {
        None,
        Solid,
        Dashed,
        Dotted,
        Custom
    } style;

    enum class EndType {
        Butt,
        Square,
        Round
    } endType;

    enum class JoinType {
        Bevel,
        Miter,
        Round
    } joinType;
};

struct aeMarkerSymbol : public aeSymbol {
    enum class Style {
        None,
        Square,
        Circle,
        Diamond,
        Cross,
        DiagonalCross,
        Custom
    } shape;

    float size;
    float angle;
    aeLineSymbol outline;
};

struct aeFillSymbol : public aeSymbol {
    enum class Style {
        None,
        Solid,
        Lines,
        Hatch,
        Custom
    } style;

    float interval;
    float angle;
    aeLineSymbol outline;
};

struct aeTextSymbol : public aeSymbol {
    class Font *font;

    enum class HorizontalAlign {
        Left,
        Center,
        Right,
        Justify
    } horizontalAlign;

    enum class VerticalAlign {
        Top,
        Middle,
        Bottom,
    } verticalAlign;

    float angle;
    float lineSpacing;
    float characterSpacing;
};

////////////////////////////////////////////////////////////////////////////////

#endif // AESYMBOL_HPP_INCLUDE_GUARD

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

