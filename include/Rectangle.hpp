#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <SFML/Graphics.hpp>
class Rectangle
{
public:
    float x;
    float y;
    float width;
    float height;

    Rectangle(float _x, float _y, float _width, float _height)
        : x(_x), y(_y), width(_width), height(_height) {}

    bool intersects(const Rectangle &other) const
    {
        if (this->x + this->width <= other.x || other.x + other.width <= this->x)
        {
            return false;
        }

        if (this->y + this->height <= other.y || other.y + other.height <= this->y)
        {
            return false;
        }

        return true;
    }
};

#endif