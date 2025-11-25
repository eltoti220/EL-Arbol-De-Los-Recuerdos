#ifndef BLOCKOFABILITY_HPP
#define BLOCKOFABILITY_HPP

#include "Ability.hpp"
#include "Player.hpp"
#include "Quadtree.hpp"

class BlockOfAbility : public Ability
{
public:
    BlockOfAbility();

    void execute(Entity *owner, Quadtree *quadtree) override;
    void update(float dtime) override;
    void draw(sf::RenderWindow &window) override;
};

#endif