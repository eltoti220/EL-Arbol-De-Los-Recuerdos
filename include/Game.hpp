#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "StateManager.hpp"
#include "GameState.hpp"
#include "MainMenu.hpp"

class Game
{
public:
    Game();
    ~Game() { delete manager; };
    void run();

private:
    sf::RenderWindow m_window;
    StateManager *manager;
    sf::Clock clock;
    void prossesEvent();
    void init();
};

#endif