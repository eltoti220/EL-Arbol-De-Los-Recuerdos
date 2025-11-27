#include "../include/PlayingState.hpp"
#include "../include/TextureManager.hpp"
#include "../include/GameMap.hpp"
#include "../include/MapaDataTypes.hpp"
#include "../include/Quadtree.hpp"
#include "../include/MeleeAttack.hpp"
#include "../include/Enemy.hpp"
#include "InventoryState.hpp"
#include <iostream>
#include <algorithm>

PlayingState::PlayingState(StateManager *manager)
    : GameState(manager), m_entityManager(nullptr), m_player(nullptr), m_gameMap(nullptr)
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
        std::vector<sf::Vector2f> waypoints = {{700.0f, 300.0f}, {950.0f, 300.0f}, {950.0f, 500.0f}};
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

        std::cout << "DEBUG: PlayingState cargó la textura usando el TextureManager.\n";
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "ERROR: No se pudo obtener la textura de fondo: " << e.what() << std::endl;
    }

    m_gameMap = new GameMap(MapaData::MAP_WIDTH_INIT, MapaData::MAP_HEIGHT_INIT);
    m_gameMap->RegisterDinamicEntity(m_player);

    m_gameView.setCenter(m_player->getPosition());
}

void PlayingState::onExit()
{
}

void PlayingState::prossesEvent(sf::Event event)
{
    // Cierra ventana
    if (event.type == sf::Event::Closed)
        manager->getWindow().close();

    // ESC para opciones / pausa
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        std::cout << "DEBUG: Entrando al OptionState.\n";
    }

    // ABRIR INVENTARIO con I
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I)
    {
        // <-- CORRECCIÓN: solo pasar manager, no m_player
        manager->pushState(new InventoryState(manager));
        return;
    }

    // ATAQUE cuerpo a cuerpo con K
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K)
    {
        MeeleAttack *attack = dynamic_cast<MeeleAttack *>(m_player->getMeeleAttack());
        Quadtree *collisionTree = m_gameMap ? m_gameMap->getCollisionTree() : nullptr;

        if (attack && collisionTree)
            attack->execute(m_player, collisionTree);
    }

    // Pasar eventos al jugador
    if (m_player)
        m_player->prossesEvent(event);
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

        // --- LÓGICA DE DAÑO DEL ENEMIGO ---
        Rectangle playerBounds = m_player->getBoundingBox();
        
        for (Entity *entity : entities)
        {
            Enemy *enemy = dynamic_cast<Enemy *>(entity);
            
            if (enemy && enemy->isAlive())
            {
                // Opción 1: Colisión de Bounding Box (simple contacto)
                // Usaremos esta por simplicidad.
                Rectangle enemyBounds = enemy->getBoundingBox();
                
                if (playerBounds.intersects(enemyBounds)) 
                {
                    // 1. Verificar si el enemigo puede atacar (cooldown listo)
                    if (enemy->isAttackReady()) 
                    {
                        // 2. Aplicar el daño al jugador (usando el daño definido en Player)
                        const int ENEMY_DAMAGE = static_cast<int>(enemy->getAttackDamage());

                        m_player->takeDamage(ENEMY_DAMAGE);
                        
                        // 3. Iniciar el cooldown del ataque del enemigo
                        enemy->startAttackCooldown(); 
                    }
                }
                
                // Opción 2: Colisión de Ataque/Rango (si el enemigo usa un AttackRadius)
                // if (enemy->distanceTo(m_player->getPosition()) <= enemy->getAttackRadius()) { ... }
            }
        }
        // --- FIN DE LA LÓGICA DE DAÑO DEL ENEMIGO ---
    }

    handleCamera(dtime);
}

void PlayingState::handleCamera(float dtime)
{
    if (!m_player || !m_gameMap)
        return;

    const float MAP_WIDTH = m_gameMap->getWidth();
    const float MAP_HEIGHT = m_gameMap->getHeight();

    sf::Vector2f playerPos = m_player->getPosition();
    sf::Vector2f viewSize = m_gameView.getSize();

    float clampedX = std::max(viewSize.x / 2.f, std::min(playerPos.x, MAP_WIDTH - viewSize.x / 2.f));
    float clampedY = std::max(viewSize.y / 2.f, std::min(playerPos.y, MAP_HEIGHT - viewSize.y / 2.f));

    m_gameView.setCenter(clampedX, clampedY);
}

void PlayingState::draw(sf::RenderWindow &window)
{
    // --- DIBUJO DEL MUNDO (usa la cámara del juego) ---
    window.setView(m_gameView);
    window.clear(sf::Color::Black);

    // Mapa
    if (m_gameMap)
        m_gameMap->draw(window);

    // Jugador
    if (m_player)
    {
        m_player->draw(window, sf::RenderStates::Default);

        // Bounding box del jugador (debug)
        Rectangle rect = m_player->getBoundingBox();
        sf::RectangleShape boundsShape(sf::Vector2f(rect.width, rect.height));
        boundsShape.setPosition(rect.x, rect.y);
        boundsShape.setFillColor(sf::Color::Transparent);
        boundsShape.setOutlineThickness(2.f);
        boundsShape.setOutlineColor(sf::Color::Yellow);
        window.draw(boundsShape);
    }

    // Entidades
    if (m_entityManager)
        m_entityManager->drawEntities(window);


    // --- DIBUJO DE LA INTERFAZ (HUD) ---
    window.setView(window.getDefaultView());  // MUY IMPORTANTE: esto fija la UI

    if (m_player)
    {
        // --- CONFIGURACIÓN DE LA BARRA ---
        const float barWidth = 200.f;
        const float barHeight = 20.f;
        const sf::Vector2f barPos(20.f, 20.f); // SIEMPRE EN LA ESQUINA IZQUIERDA

        // Fondo de la barra
        sf::RectangleShape healthBarBack(sf::Vector2f(barWidth, barHeight));
        healthBarBack.setFillColor(sf::Color(50, 50, 50));
        healthBarBack.setPosition(barPos);
        window.draw(healthBarBack);

        // Porcentaje de vida
        float healthPercent =
            static_cast<float>(m_player->getCurrentHealth()) /
            static_cast<float>(m_player->getMaxHealth());

        healthPercent = std::max(0.f, std::min(1.f, healthPercent)); // clamp

        // Barra roja
        sf::RectangleShape healthBar(sf::Vector2f(barWidth * healthPercent, barHeight));
        healthBar.setFillColor(sf::Color::Red);
        healthBar.setPosition(barPos);
        window.draw(healthBar);
    }
}

