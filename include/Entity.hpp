#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Rectangle.hpp"

class Entity : public sf::Drawable
{
public:
    virtual ~Entity() = default;
    virtual void update(float dtime) = 0;
    virtual Rectangle getBoundingBox() const = 0;
    sf::Vector2f getPosition() const { return m_position; }
    void setPosition(float x, float y) { m_position = sf::Vector2f(x, y); }

protected:
    sf::Vector2f m_position;
    float m_speed = 0.0f;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
};

#endif