#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/Graphics.hpp>

class StateManager;

class GameState
{
public:
    StateManager *manager;
    GameState(StateManager *_manager) : manager(_manager) {}
    virtual ~GameState();

    virtual void prossesEvent(sf::Event event) = 0;
    virtual void update(float dtime) = 0;
    virtual void draw(sf::RenderWindow &window) = 0;

    virtual void onEntry();
    virtual void onExit();

    virtual void handleInput() {}
    virtual void render() {}  
};

#endif