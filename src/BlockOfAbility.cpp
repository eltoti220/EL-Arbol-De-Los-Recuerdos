#include "../include/BlockOfAbility.hpp"
#include <iostream>

BlockOfAbility::BlockOfAbility()
{
    cooldown = 0.0f;
    cooldownTimer = 0.0f;
}

void BlockOfAbility::execute(Entity *owner, Quadtree *quadtree)
{
    Player *player = dynamic_cast<Player *>(owner);
    if (player)
    {
        player->setBlocking(false);
    }
}

void BlockOfAbility::update(float dtime)
{
    // No hace nada, ol bloqueo al ser mientras se presiona con una tecla o mejor dicho, al ser un evento de una vez
}

void BlockOfAbility::draw(sf::RenderWindow &window)
{
    // va en Player::draw
}