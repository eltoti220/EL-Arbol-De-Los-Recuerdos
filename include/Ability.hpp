#ifndef ABILITY_HPP
#define ABILITY_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class Entity;

class Ability
{
public:
    Ability() = default;
    virtual ~Ability() = default;
    virtual void execute(Entity *owner) = 0;
    virtual void update(float dtime) = 0;
    virtual bool isReady() const { return cooldownTimer <= 0.0f; }
    virtual void draw(sf::RenderWindow &window) {}

protected:
    float damage = 0.0f;
    float cooldown = 0.0f;
    float cooldownTimer = 0.0f;
};

#endif