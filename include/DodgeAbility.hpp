#ifndef DODGEABILITY_HPP
#define DODGEABILITY_HPP

#include "Ability.hpp"
#include "Player.hpp"

class DodgeAbility : public Ability
{
public:
    DodgeAbility();

    void execute(Entity *owner) override;
    void update(float dtime) override;
    void draw(sf::RenderWindow &window) override;

private:
    float dodgeDuration = 0.2f;
};

#endif