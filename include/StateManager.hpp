#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <vector>
#include <memory>
#include "GameState.hpp"
#include "InventoryData.hpp"

class StateManager
{
public:
    StateManager(sf::RenderWindow &_window);
    ~StateManager();

    // Métodos
    void pushState(GameState *state);
    void popState();
    GameState* getCurrentState();
    void changeState(GameState *state);
    void prossesEvent(sf::Event event);
    void update(float dtime);
    void draw(sf::RenderWindow &window);
    bool isEmpty() const;

    sf::RenderWindow &getWindow() { return window; }

    // Inventario global
    InventoryData inventory;

private:
    std::vector<GameState *> StackState;
    sf::RenderWindow &window;
};

#endif