#ifndef MELEEATTACK_HPP
#define MELEEATTACK_HPP

#include "Ability.hpp"
#include "Entity.hpp"

class MeeleAttack : public Ability
{
public:
    MeeleAttack();
    void execute(Entity *owner) override;
    void update(float dtime) override;
    void draw(sf::RenderWindow &window) override;

private:
    int secuenceStep;
    float secuenceStepTimer;
    float getDamageForStep(int step) const;
    const float COOLDOWN_NORMAL = 0.4f;
    const float COOLDOWN_SPECIAL = 0.9f;
};

#endif