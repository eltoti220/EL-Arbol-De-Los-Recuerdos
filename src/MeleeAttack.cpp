#include "../include/MeleeAttack.hpp"
#include <iostream>

MeeleAttack::MeeleAttack()
{
    cooldown = 1.0f;
    cooldownTimer = 0.0f;
    secuenceStep = 0;
    secuenceStepTimer = 0.0;
}

float MeeleAttack::getDamageForStep(int step) const
{
    if (step == 1)
    {
        return 10.0f;
    }
    if (step == 2)
    {
        return 10.0f;
    }
    if (step == 3)
    {
        return 20.0f;
    }
    return 0.0;
}

void MeeleAttack::execute(Entity *owner)
{
    if (!isReady())
    {
        return;
    }

    if (secuenceStepTimer > 0.0f)
    {
        secuenceStep++;
    }
    else
    {
        secuenceStep = 1;
    }

    std::cout << "DEBUG: Ataque Melee #" << secuenceStep << " ejecutado. Daño: "
              << getDamageForStep(secuenceStep) << " | Posición: "
              << owner->getPosition().x << std::endl;

    // creo que por aqui iria la colicion o hitbox, pero hay que revisarlo bien

    if (secuenceStep == 3)
    {

        cooldownTimer = COOLDOWN_SPECIAL;
        secuenceStep = 0;
        secuenceStepTimer = 0.0f;
        std::cout << "DEBUG: secuencia de ataque reiniciada" << std::endl;
    }
    else
    {
        cooldownTimer = COOLDOWN_NORMAL;
        secuenceStepTimer = 0.69f;
    }
}

void MeeleAttack::update(float dtime)
{
    if (cooldownTimer > 0.0f)
    {
        cooldownTimer -= dtime;
    }

    if (secuenceStepTimer > 0.0f)
    {
        secuenceStepTimer -= dtime;
        if (secuenceStepTimer <= 0.0f)
        {
            std::cout << "DEBUG: secuencia de ataque reiniciada" << std::endl;
            secuenceStep = 0;
        }
    }
}

void MeeleAttack::draw(sf::RenderWindow &window)
{
    // se dibujara el ataque
}
