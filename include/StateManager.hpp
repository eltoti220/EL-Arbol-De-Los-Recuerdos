#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <vector>
#include <memory>

#include "GameState.hpp"

class StateManager
{
public:
    StateManager(sf::RenderWindow &_window);
    sf::RenderWindow &getWindow() { return window; }
    ~StateManager();
    void pushSate(GameState *state);
    void popState();
    void changeState(GameState *state);

    void prossesEvent(sf::Event event);
    void update(float dtime);
    void draw(sf::RenderWindow &window);
    bool isEmpty() const;

private:
    std::vector<GameState *> StackState;
    sf::RenderWindow &window;
};

#endif