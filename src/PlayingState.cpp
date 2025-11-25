#include "../include/PlayingState.hpp"
#include "../include/TextureManager.hpp"
#include "../include/GameMap.hpp"
#include "../include/MapaDataTypes.hpp"
#include "../include/Quadtree.hpp"
#include "../include/MeleeAttack.hpp"
#include "../include/Enemy.hpp"
#include <iostream>
#include <algorithm>

PlayingState::PlayingState(StateManager *manager) : GameState(manager), m_entityManager(nullptr), m_player(nullptr), m_gameMap(nullptr)
{
    m_gameView.setSize(manager->getWindow().getSize().x, manager->getWindow().getSize().y);
}

PlayingState::~PlayingState()
{
    delete m_entityManager;
    delete m_gameMap;
}

void PlayingState::onEntry()
{
    m_entityManager = new EntityManager();
    m_player = new Player(850.0f, 370.0f);
    m_entityManager->addEntity(m_player);

    try
    {
        std::string enemyTextureId = "Enemy_Goblin";

        std::vector<sf::Vector2f> waypoints = {
            {700.0f, 300.0f},
            {950.0f, 300.0f},
            {950.0f, 500.0f}};

        Enemy *m_testEnemy = new Enemy(waypoints[0], waypoints, enemyTextureId, TextureManager::getInstance());

        m_entityManager->addEntity(m_testEnemy);
        std::cout << "DEBUG: Se ha agregado un enemigo de prueba al EntityManager.\n";
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "ERROR: No se pudo crear o agregar el enemigo: " << e.what() << std::endl;
    }

    try
    {
        sf::Texture &tx_background = TextureManager::getInstance().getTexture("Background_mapa_Init");
        sp_background.setTexture(tx_background);
        sp_background.setScale(7.0f, 5.0f);
        std::cout << "DEBUG: PlayingState Cargo la textura usando el texturemanager.\n";
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "ERROR: No se pudo obtener la textura de fondo ('Background_mapa_Init') del manager: " << e.what() << std::endl;
    }

    m_gameMap = new GameMap(MapaData::MAP_WIDTH_INIT, MapaData::MAP_HEIGHT_INIT);
    m_gameMap->RegisterDinamicEntity(m_player);

    m_gameView.setCenter(m_player->getPosition());
}

void PlayingState::onExit()
{
    // std::cout << "DEBUG: Saliendo del PlayingState.\n";
    manager->popState();
}

void PlayingState::prossesEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        // manager->pushState(new PauseMenu(manager));
        std::cout << "DEBUG: Entrando al OptionState.\n";
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K)
    {
        MeeleAttack *attack = dynamic_cast<MeeleAttack *>(m_player->getMeeleAttack());
        Quadtree *collisionTree = m_gameMap ? m_gameMap->getCollisionTree() : nullptr;
        if (attack && collisionTree)
        {
            attack->execute(m_player, collisionTree);
        }
    }

    if (m_player)
    {
        m_player->prossesEvent(event);
    }
}

void PlayingState::update(float dtime)
{
    if (m_entityManager && m_gameMap)
    {
        m_entityManager->updateEntities(dtime, *m_player);
        m_gameMap->update(dtime);

        const std::vector<Entity *> &entities = m_entityManager->getEntities();
        for (Entity *entity : entities)
        {
            m_gameMap->RegisterDinamicEntity(entity);
        }

        // aqui se aniadirira la logica central de la colision entre entidades creo (osea  jugaodr vs enemigo)
        // la consulta de isDodging o isBlocking se realizara en esa logica
    }
    handleCamera(dtime);
}

void PlayingState::handleCamera(float dtime)
{
    if (m_player)
    {
        const float MAP_WIDTH = m_gameMap->getWidth();
        const float MAP_HEIGHT = m_gameMap->getHeight();

        sf::Vector2f playerPos = m_player->getPosition();
        sf::Vector2f viewSize = m_gameView.getSize();

        float clampedX = playerPos.x;
        float clampedY = playerPos.y;

        if (MAP_WIDTH > viewSize.x)
        {
            float minX = viewSize.x / 2.0f;
            float maxX = MAP_WIDTH - (viewSize.x / 2.0f);
            clampedX = std::max(minX, std::min(playerPos.x, maxX));
        }
        else
        {
            clampedX = MAP_WIDTH / 2.0f;
        }

        if (MAP_HEIGHT > viewSize.y)
        {
            float minY = viewSize.y / 2.0f;
            float maxY = MAP_HEIGHT - (viewSize.y / 2.0f);
            clampedY = std::max(minY, std::min(playerPos.y, maxY));
        }
        else
        {
            clampedY = MAP_HEIGHT / 2.0f;
        }

        m_gameView.setCenter(clampedX, clampedY);
    }
}

void PlayingState::draw(sf::RenderWindow &window)
{
    window.setView(m_gameView);
    window.clear(sf::Color::Black);
    if (m_gameMap)
    {
        m_gameMap->draw(window);
    }

    // =========================================================
    // DEBUG: DIBUJAR LA BOUNDING BOX DEL JUGADOR
    // =========================================================
    if (m_player)
    {
        // 1. Obtener la BoundingBox (asumo que Player tiene getBoundingBox())
        // ¡DEBES asegurarte de que este método exista en Player/Entity!
        Rectangle rect = m_player->getBoundingBox();

        // 2. Crear un sf::RectangleShape para dibujarla
        sf::RectangleShape boundsShape;
        // Asumiendo Rectangle tiene width/height
        boundsShape.setSize(sf::Vector2f(rect.width, rect.height));
        // Asumiendo Rectangle tiene x/y para la posición superior-izquierda
        boundsShape.setPosition(rect.x, rect.y);

        boundsShape.setFillColor(sf::Color::Transparent);
        boundsShape.setOutlineThickness(2.0f);
        boundsShape.setOutlineColor(sf::Color::Yellow); // Un color que resalte

        window.draw(boundsShape);

        // Opcional: Imprimir las coordenadas en la consola
        std::cout << "DEBUG: Player BoundingBox: X=" << rect.x << ", Y=" << rect.y
                  << ", W=" << rect.width << ", H=" << rect.height << "\r";
    }

    if (m_entityManager)
    {
        m_entityManager->drawEntities(window);
    }
    window.setView(window.getDefaultView());
}
