#include "../include/GameMap.hpp"
#include "../include/TextureManager.hpp"
#include <iostream>
const float SUBDIVISION_SIZE = 100.0f;

GameMap::GameMap(float width, float height)
    : m_width(width), m_height(height), m_collisionTree(nullptr)
{

    MapData mapData;
    const std::string MAP_FILE = "assets/metadata/mapa_init.json";

    try
    {
        mapData = MapLoader::loadMap(MAP_FILE);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error Fatal: No e pudo cargar el mapa desde el json (" << MAP_FILE << "). Detalles:" << e.what() << std::endl;
    }

    m_width = mapData.width;
    m_height = mapData.height;

    Rectangle boundary(0.0f, 0.0f, m_width, m_height);
    m_collisionTree = new Quadtree(boundary, 6, 5);

    try
    {
        sf::Texture &tx_background = TextureManager::getInstance().getTexture(mapData.backgroundTextureId);
        m_backgroundSprite.setTexture(tx_background);
        m_backgroundSprite.setScale(7.0f, 5.0f);
        m_foregroundSprite.setTexture(TextureManager::getInstance().getTexture(mapData.type_tree));
        m_foregroundSprite.setScale(7.0f, 5.0f);

        std::cout << "DEBUG: GameMap Cargo la textura de fondo usando el texturemanager.\n";
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "ERROR: No se pudo obtener la textura de fondo ('Background_mapa_Init') del manager: " << e.what() << std::endl;
    }
    std::cout << "DEBUG: GameMap inicializado. Dimensiones: " << m_width << "x" << m_height << std::endl;

    for (const auto &data : mapData.staticObstacles)
    {
        int numX = static_cast<int>(std::ceil(data.width / SUBDIVISION_SIZE));
        int numY = static_cast<int>(std::ceil(data.height / SUBDIVISION_SIZE));

        for (int j = 0; j < numY; ++j)
        {
            for (int i = 0; i < numX; ++i)
            {
                float sub_x = data.x + i * SUBDIVISION_SIZE;
                float sub_y = data.y + j * SUBDIVISION_SIZE;

                float sub_w = std::min(SUBDIVISION_SIZE, data.width - i * SUBDIVISION_SIZE);
                float sub_h = std::min(SUBDIVISION_SIZE, data.height - j * SUBDIVISION_SIZE);

                if (sub_w <= 0.0f || sub_h <= 0.0f)
                    continue;

                try
                {
                    auto newTree = std::make_unique<Tree>(sub_x, sub_y, sub_w, sub_h, data.textureId);
                    m_collisionTree->insert(newTree.get());
                    m_staticEntities.push_back(std::move(newTree));
                }
                catch (const std::exception &e)
                {
                    std::cerr << "ERROR al crear sub-obstáculo (" << data.id << "): " << e.what() << std::endl;
                }
            }
        }
    }
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
        Rectangle boundary = m_collisionTree->getBoundary();
        delete m_collisionTree;

        m_collisionTree = new Quadtree(boundary, 6, 5);

        for (const auto &entity : m_staticEntities)
        {
            m_collisionTree->insert(entity.get());
        }

        for (Entity *entity : m_dynamicEntities)
        {
            m_collisionTree->insert(entity);
        }
    }
}

void GameMap::RegisterDinamicEntity(Entity *entity)
{
    bool alreadyRegistered = false;
    for (Entity *entity : m_dynamicEntities)
    {
        if (entity == entity)
        {
            alreadyRegistered = true;
            break;
        }
    }

    if (!alreadyRegistered)
    {
        m_dynamicEntities.push_back(entity);
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

    for (const auto &entity : m_staticEntities)
    {
        window.draw(*entity);
    }

    if (m_collisionTree)
    {
        m_collisionTree->draw(window);
    }
}
