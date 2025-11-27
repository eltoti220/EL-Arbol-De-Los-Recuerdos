
#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Quadtree.hpp"
#include "Entity.hpp"
#include "MapaDataTypes.hpp"
#include "Rectangle.hpp"
#include "Tree.hpp"
#include <stdexcept>

class GameMap
{
public:
    GameMap(float width, float height);
    ~GameMap();

    void update(float dtime);
    void draw(sf::RenderWindow &window);
    void RegisterDinamicEntity(Entity *entity);
    Quadtree *getCollisionTree() { return m_collisionTree; }
    std::vector<Entity *> getEntitiesInArea(const Rectangle &area) const;

    // geters de dimensiones del mapa
    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }

private:
    float m_width;
    float m_height;
    Quadtree *m_collisionTree;
    sf::Sprite m_backgroundSprite;
    sf::Sprite m_foregroundSprite;
    std::vector<std::unique_ptr<Entity>> m_staticEntities;
    std::vector<Entity *> m_dynamicEntities;
};

#endif