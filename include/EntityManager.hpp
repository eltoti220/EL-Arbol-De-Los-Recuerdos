#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include "Entity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

class EntityManager
{
public:
    ~EntityManager();
    void addEntity(Entity *entity);
    void updateEntities(float dtime, Player& player);
    void drawEntities(sf::RenderWindow &window);

private:
    std::vector<Entity *> entities;
};

#endif