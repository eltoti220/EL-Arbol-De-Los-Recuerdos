#include "../include/BlockOnAbility.hpp"
#include <iostream>

BlockOnAbility::BlockOnAbility()
{
    cooldown = 0.0f;
    cooldownTimer = 0.0f;
}

void BlockOnAbility::execute(Entity *owner, Quadtree *quadtree)
{
    Player *player = dynamic_cast<Player *>(owner);
    if (player)
    {
        player->setBlocking(true);
    }
}

void BlockOnAbility::update(float dtime)
{
    // No hace nada, ol bloqueo al ser mientras se presiona con una tecla o mejor dicho, al ser un evento de una vez
}

void BlockOnAbility::draw(sf::RenderWindow &window)
{
    // va en Player::draw
}