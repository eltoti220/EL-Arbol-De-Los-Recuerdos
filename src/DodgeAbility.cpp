#include "../include/DodgeAbility.hpp"
#include "../include/Player.hpp"

DodgeAbility::DodgeAbility()
{
    cooldown = 1.2f;
    cooldownTimer = 0.0f;
    dodgeDuration = 0.15f;
}

void DodgeAbility::execute(Entity *owner, Quadtree *quadtree)
{
    if (!isReady())
    {
        return;
    }

    cooldownTimer = cooldown;
    Player *player = dynamic_cast<Player *>(owner);
    if (player)
    {
        player->startDodge(dodgeDuration);
        std::cout << "DEBUG: Esquiva iniciada por " << dodgeDuration << " segundos.\n";
    }
}

void DodgeAbility::update(float dtime)
{
    if (cooldownTimer > 0.0f)
    {
        cooldownTimer -= dtime;
    }
}

void DodgeAbility::draw(sf::RenderWindow &window)
{
    // animacion de parar con el hacha
}