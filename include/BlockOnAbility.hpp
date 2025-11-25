#ifndef BLOCKONABILITY_HPP
#define BLOCKONABILITY_HPP

#include "Ability.hpp"
#include "Player.hpp"

class BlockOnAbility : public Ability
{
public:
    BlockOnAbility();

    void execute(Entity *owner) override;
    void update(float dtime) override;
    void draw(sf::RenderWindow &window) override;
};

#endif