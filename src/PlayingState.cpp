#include "../include/PlayingState.hpp"
#include "../include/TextureManager.hpp"
#include "../include/Enemy.hpp"
#include <iostream>

PlayingState::PlayingState(StateManager *manager) : GameState(manager), m_entityManager(nullptr), m_player(nullptr)
{
    m_gameView.setSize(manager->getWindow().getSize().x, manager->getWindow().getSize().y);
}

PlayingState::~PlayingState()
{
    delete m_entityManager; 
}

void PlayingState::onEntry()
{
    m_entityManager = new EntityManager();
    
    m_player = new Player(850.0f, 370.0f);
    m_entityManager->addEntity(m_player);

    /*
    try {
        std::string enemyTextureId = "Enemy_Goblin"; 
        
        std::vector<sf::Vector2f> waypoints = {
            {700.0f, 300.0f},
            {950.0f, 300.0f},
            {950.0f, 500.0f}
        };

        Enemy *m_testEnemy = new Enemy(waypoints[0], 
                                       waypoints, 
                                       enemyTextureId, 
                                       TextureManager::getInstance());

        m_entityManager->addEntity(m_testEnemy);
        std::cout << "DEBUG: Se ha agregado un enemigo de prueba al EntityManager.\n";

    } catch (const std::runtime_error &e) {
        std::cerr << "ERROR: No se pudo crear o agregar el enemigo: " << e.what() << std::endl;
    }
        */

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

    m_gameView.setCenter(m_player->getPosition());
}

void PlayingState::onExit()
{
    
}

void PlayingState::prossesEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        
    }

    if (m_player)
    {
        m_player->prossesEvent(event);
    }
}

void PlayingState::update(float dtime)
{
   
    if (m_entityManager)
    {
        m_entityManager->updateEntities(dtime, *m_player);
    }
    handleCamera(dtime);
}

void PlayingState::handleCamera(float dtime)
{
    if (m_player)
    {
        const float MAP_WIDTH = 256.0f * 7.0f;
        const float MAP_HEIGHT = 144.0f * 5.0f;

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
    window.draw(sp_background);

    if (m_entityManager)
    {
        m_entityManager->drawEntities(window);
    }
    window.setView(window.getDefaultView());
}