#include "../include/Quadtree.hpp"
#include "../include/Entity.hpp"
#include <algorithm>

Quadtree::Quadtree(const Rectangle &boundary, int capacity, int level)
    : m_boundary(boundary), m_capacity(capacity), m_level(level),
      m_northwest(nullptr), m_northeast(nullptr), m_southwest(nullptr), m_southeast(nullptr)
{
}

void Quadtree::clear()
{
    m_entities.clear();
    if (m_northwest)
    {
        m_northwest->clear();
        m_northeast->clear();
        m_southwest->clear();
        m_southeast->clear();

        m_northwest.reset();
        m_northeast.reset();
        m_southwest.reset();
        m_southeast.reset();
    }
}

void Quadtree::subdivide()
{
    float halfWidth = m_boundary.width / 2.0f;
    float halfHeight = m_boundary.height / 2.0f;
    float x = m_boundary.x;
    float y = m_boundary.y;
    int newLevel = m_level + 1;

    m_northwest = std::make_unique<Quadtree>(Rectangle(x, y, halfWidth, halfHeight), m_capacity, newLevel);
    m_northeast = std::make_unique<Quadtree>(Rectangle(x + halfWidth, y, halfWidth, halfHeight), m_capacity, newLevel);
    m_southwest = std::make_unique<Quadtree>(Rectangle(x, y + halfHeight, halfWidth, halfHeight), m_capacity, newLevel);
    m_southeast = std::make_unique<Quadtree>(Rectangle(x + halfWidth, y + halfHeight, halfWidth, halfHeight), m_capacity, newLevel);
}

bool Quadtree::insert(Entity *entity)
{
    Rectangle entityRect = entity->getBoundingBox();

    if (!m_boundary.intersects(entityRect))
    {
        return false;
    }

    if (m_northwest)
    {
        if (m_northwest->getBoundary().contains(entityRect))
        {
            return m_northwest->insert(entity);
        }
        else if (m_northeast->getBoundary().contains(entityRect))
        {
            return m_northeast->insert(entity);
        }
        else if (m_southwest->getBoundary().contains(entityRect))
        {
            return m_southwest->insert(entity);
        }
        else if (m_southeast->getBoundary().contains(entityRect))
        {
            return m_southeast->insert(entity);
        }
    }

    if (!m_northwest && m_entities.size() < m_capacity)
    {
        m_entities.push_back(entity);
        return true;
    }

    if (m_level < MAX_LEVELS && !m_northwest)
    {
        subdivide();

        for (auto it = m_entities.begin(); it != m_entities.end();)
        {
            Entity *existingEntity = *it;
            Rectangle existingRect = existingEntity->getBoundingBox();

            if (m_northwest->getBoundary().contains(existingRect))
            {
                m_northwest->insert(existingEntity);
                it = m_entities.erase(it);
            }
            else if (m_northeast->getBoundary().contains(existingRect))
            {
                m_northeast->insert(existingEntity);
                it = m_entities.erase(it);
            }
            else if (m_southwest->getBoundary().contains(existingRect))
            {
                m_southwest->insert(existingEntity);
                it = m_entities.erase(it);
            }
            else if (m_southeast->getBoundary().contains(existingRect))
            {
                m_southeast->insert(existingEntity);
                it = m_entities.erase(it);
            }
            else
            {
                ++it;
            }
        }

        if (m_northwest->insert(entity) || m_northeast->insert(entity) || m_southwest->insert(entity) || m_southeast->insert(entity))
        {
            return true;
        }
    }

    // 5. Si no cabe en ningún sub-nodo (es demasiado grande o cruza las fronteras),
    // se queda almacenado en este nodo. Esto es CRÍTICO para los objetos grandes.
    m_entities.push_back(entity);
    return true;
}

std::vector<Entity *> Quadtree::query(const Rectangle &range) const
{
    std::vector<Entity *> foundEntities;

    if (!m_boundary.intersects(range))
    {
        return foundEntities;
    }

    for (Entity *entity : m_entities)
    {
        Rectangle entityRect = entity->getBoundingBox();
        if (range.intersects(entityRect))
        {
            foundEntities.push_back(entity);
        }
    }

    if (m_northwest)
    {

        if (m_northwest->getBoundary().intersects(range))
        {
            std::vector<Entity *> nwEntities = m_northwest->query(range);
            foundEntities.insert(foundEntities.end(), nwEntities.begin(), nwEntities.end());
        }
        if (m_northeast->getBoundary().intersects(range))
        {
            std::vector<Entity *> neEntities = m_northeast->query(range);
            foundEntities.insert(foundEntities.end(), neEntities.begin(), neEntities.end());
        }
        if (m_southwest->getBoundary().intersects(range))
        {
            std::vector<Entity *> swEntities = m_southwest->query(range);
            foundEntities.insert(foundEntities.end(), swEntities.begin(), swEntities.end());
        }
        if (m_southeast->getBoundary().intersects(range))
        {
            std::vector<Entity *> seEntities = m_southeast->query(range);
            foundEntities.insert(foundEntities.end(), seEntities.begin(), seEntities.end());
        }
    }

    return foundEntities;
}

void Quadtree::draw(sf::RenderWindow &window) const
{
    sf::RectangleShape rectShape;
    rectShape.setPosition(m_boundary.x, m_boundary.y);
    rectShape.setSize(sf::Vector2f(m_boundary.width, m_boundary.height));
    rectShape.setFillColor(sf::Color::Transparent);
    rectShape.setOutlineColor(sf::Color::Red);
    rectShape.setOutlineThickness(1.0f);
    window.draw(rectShape);

    for (Entity *entity : m_entities)
    {
        Rectangle rect = entity->getBoundingBox();
        sf::RectangleShape entityShape;
        entityShape.setSize(sf::Vector2f(rect.width, rect.height));
        entityShape.setPosition(rect.x, rect.y);
        entityShape.setFillColor(sf::Color::Transparent);

        // 🔑 Color VERDE para las Bounding Boxes de las entidades
        entityShape.setOutlineColor(sf::Color::Green);
        entityShape.setOutlineThickness(2.0f); // Más grueso para que resalte
        window.draw(entityShape);
    }

    if (m_northwest)
    {
        m_northwest->draw(window);
        m_northeast->draw(window);
        m_southwest->draw(window);
        m_southeast->draw(window);
    }
}