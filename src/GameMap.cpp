#include "../include/GameMap.hpp"
#include "../include/TextureManager.hpp"
#include <iostream>

GameMap::GameMap(float width, float height)
    : m_width(width), m_height(height), m_collisionTree(nullptr)
{
    Rectangle boundary(0.0f, 0.0f, m_width, m_height);
    m_collisionTree = new Quadtree(boundary, 4);

    try
    {
        sf::Texture &tx_background = TextureManager::getInstance().getTexture("Background_mapa_Init");
        m_backgroundSprite.setTexture(tx_background);
        m_backgroundSprite.setScale(7.0f, 5.0f);

        std::cout << "DEBUG: GameMap Cargo la textura de fondo usando el texturemanager.\n";
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "ERROR: No se pudo obtener la textura de fondo ('Background_mapa_Init') del manager: " << e.what() << std::endl;
    }
    std::cout << "DEBUG: GameMap inicializado. Dimensiones: " << m_width << "x" << m_height << std::endl;
}

GameMap::~GameMap()
{
    delete m_collisionTree;
    m_collisionTree = nullptr;
    std::cout << "DEBUG: GameMap destruido y memoria liberada.\n";
}

void GameMap::update(float dtime)
{
    // Aquí se podrían actualizar elementos del mapa si es necesario como las entidades
    if (m_collisionTree)
    {
        m_collisionTree->clear();
    }
}

void GameMap::RegisterDinamicEntity(Entity *entity)
{
    if (m_collisionTree)
    {
        if (m_collisionTree->insert(entity))
        {
        }
        else
        {
            std::cout << "WARNING: La entidad no pudo ser registrada en el Quadtree de colisiones del GameMap (fuera de los límites).\n";
        }
    }
}

std::vector<Entity *> GameMap::getEntitiesInArea(const Rectangle &area) const
{
    if (m_collisionTree)
    {
        return m_collisionTree->query(area);
    }
    return std::vector<Entity *>();
}

void GameMap::draw(sf::RenderWindow &window)
{
    window.draw(m_backgroundSprite);

    if (m_collisionTree)
    {
        m_collisionTree->draw(window);
    }
}
