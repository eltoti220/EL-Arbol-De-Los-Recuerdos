#include "../include/EntityManager.hpp"

EntityManager::~EntityManager()
{
    std::cout << "DEBUG: Liberando memoria de todas las entidades.\n";
    for (Entity *entity : entities)
    {
        delete entity;
    }
    entities.clear();
}

void EntityManager::addEntity(Entity *entity)
{
    if (entity)
    {
        entities.push_back(entity);
    }
}

void EntityManager::updateEntities(float dtime, Player& player)
{
    for (Entity* entity : entities)
    {
        if (Enemy* enemy = dynamic_cast<Enemy*>(entity))
        {
            enemy->update(dtime, player);   
        }
        else
        {
            entity->update(dtime);
        }
    }
}

void EntityManager::drawEntities(sf::RenderWindow &window)
{
    for (Entity *entity : entities)
    {
        window.draw(*entity);
    }
}