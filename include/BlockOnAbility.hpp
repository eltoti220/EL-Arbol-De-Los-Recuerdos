#ifndef BLOCKONABILITY_HPP
#define BLOCKONABILITY_HPP

#include "Ability.hpp"
#include "Player.hpp"
#include "Quadtree.hpp"

class BlockOnAbility : public Ability
{
public:
    BlockOnAbility();

    void execute(Entity *owner, Quadtree *quadtree) override;
    void update(float dtime) override;
    void draw(sf::RenderWindow &window) override;
};

#endif