#include "../include/Game.hpp"
#include "../include/TextureManager.hpp"

Game::Game()
{
    init();
    m_window.setFramerateLimit(60);
    manager = new StateManager(m_window);
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
    TextureManager::getInstance().loadTexture("Menu_Background", "assets/graphics/assets_Menu/Menu_Background.png");
    TextureManager::getInstance().loadTexture("Background_mapa_Init", "assets/graphics/Mapas/Background_mapa_Init.png");
}
