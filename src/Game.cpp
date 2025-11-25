#include "../include/Game.hpp"
#include "../include/TextureManager.hpp"
#include "../include/PlayingState.hpp" // Necesitas esto para MainMenu eventualmente cambie a PlayingState

Game::Game()
{
    // 1. La ventana debe crearse primero.
    init(); 
    
    // 2. Ahora que la ventana existe, inicializamos el StateManager.
    manager = new StateManager(m_window);

    m_window.setFramerateLimit(60);
    
    // 3. Empujar el estado inicial.
    manager->pushSate(new MainMenu(manager)); 
}

void Game::run()
{
    while (m_window.isOpen() && !manager->isEmpty())
    {
        sf::Time time = clock.restart();
        float dtsecond = time.asSeconds();
        prossesEvent();

        manager->update(dtsecond);
        // La limpieza (window.clear()) debe hacerse antes del manager->draw.
        m_window.clear(); 
        manager->draw(m_window);
        m_window.display();
    }
}

void Game::prossesEvent()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
            // Esto es redundante si `window.close()` se maneja correctamente en el bucle principal, 
            // pero lo dejamos para asegurar el vaciado.
            while (!manager->isEmpty())
            {
                manager->popState();
            }
            return;
        }
        manager->prossesEvent(event);
    }
}

void Game::init()
{
    m_window.create(sf::VideoMode(1400, 700), "Menu");
    
    // --- CARGA DE TEXTURAS ---
    TextureManager::getInstance().loadTexture("Menu_Background", "assets/graphics/assets_Menu/Menu_Background.png");
    TextureManager::getInstance().loadTexture("Background_mapa_Init", "assets/graphics/Mapas/Background_mapa_Init.png");
    
    // ¡AÑADIR TEXTURAS CLAVE DEL JUEGO!
    // 1. Textura del Enemigo (Usamos "Enemy_Goblin" como ID)
    TextureManager::getInstance().loadTexture("Enemy_Goblin", "assets/graphics/Characters/enemy_goblin.png"); 

    // 2. Textura del Jugador (Crucial para el Player en PlayingState)
    TextureManager::getInstance().loadTexture("Player_Sprite", "assets/graphics/Characters/player_main.png"); 
    
    std::cout << "DEBUG: Texturas iniciales cargadas en Game::init().\n";
    // -------------------------
}