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

    sf::Vector2f CalculateMtv(const Rectangle &other) const
    {
        float dx1 = (other.x + other.width) - this->x;  // from left
        float dx2 = (this->x + this->width) - other.x;  // from right
        float dy1 = (other.y + other.height) - this->y; // from top
        float dy2 = (this->y + this->height) - other.y; // from bottom

        float mtvX = (dx1 < dx2) ? dx1 : -dx2;
        float mtvY = (dy1 < dy2) ? dy1 : -dy2;

        if (std::abs(mtvX) < std::abs(mtvY))
        {
            return sf::Vector2(mtvX, 0.0f);
        }
        else
        {
            return sf::Vector2(0.0f, mtvY);
        }
    }

    bool contains(const Rectangle point) const
    {
        bool x_in = point.x >= this->x && (point.x + point.width) <= (this->x + this->width);
        bool y_in = point.y >= this->y && (point.y + point.height) <= (this->y + this->height);
        return x_in && y_in;
    }
};

#endif