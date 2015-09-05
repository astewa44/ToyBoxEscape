//
// Created by Anthony on 9/4/2015.
//

#ifndef TOYBOXESCAPE_DEBUGDRAW_HPP
#define TOYBOXESCAPE_DEBUGDRAW_HPP

#include <Box2D/Box2D.h>
#include <sfml/Graphics.hpp>

class DebugDraw : public b2Draw
{
private:
    sf::RenderWindow* Window;
    float PixelsPerMeter;
    float MetersPerPixel;

public:
    DebugDraw(sf::RenderWindow &window, float ratio) :
        Window(&window),
        PixelsPerMeter(ratio),
        MetersPerPixel(1.f / ratio)
    { }

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);
    sf::Color B2SFColor(const b2Color &color, int alpha = 255);
};


#endif //TOYBOXESCAPE_DEBUGDRAW_HPP
