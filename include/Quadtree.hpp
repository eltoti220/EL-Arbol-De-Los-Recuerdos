#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Rectangle.hpp"

class Entity;

class Quadtree
{
public:
    Quadtree(const Rectangle &boundary, int capacity, int level = 0);
    ~Quadtree() = default;

    void clear();

    bool insert(Entity *entity);

    std::vector<Entity *> query(const Rectangle &range) const;
    void draw(sf::RenderWindow &window) const;

private:
    int m_capacity;
    Rectangle m_boundary;
    std::vector<Entity *> m_entities;
    int m_level;
    const int MAX_LEVELS = 5;

    std::unique_ptr<Quadtree> m_northwest;
    std::unique_ptr<Quadtree> m_northeast;
    std::unique_ptr<Quadtree> m_southwest;
    std::unique_ptr<Quadtree> m_southeast;

    void subdivide();
};

#endif