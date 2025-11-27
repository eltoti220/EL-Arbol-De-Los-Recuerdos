#include "../include/StateManager.hpp"

StateManager::StateManager(sf::RenderWindow &_window) : window(_window) {}

StateManager::~StateManager()
{
    while (!StackState.empty())
    {
        popState();
    }
}

void StateManager::pushState(GameState *state)
{
    StackState.push_back(state);
    state->onEntry();
}

void StateManager::popState()
{
    if (!StackState.empty())
    {
        StackState.back()->onExit();
        delete StackState.back();
        StackState.pop_back();
    }
}

void StateManager::changeState(GameState *state)
{
    if (!StackState.empty())
    {
        popState();
    }
    pushState(state);
}

void StateManager::prossesEvent(sf::Event event)
{
    if (!StackState.empty())
    {
        StackState.back()->prossesEvent(event);
    }
}

void StateManager::update(float dtime)
{
    if (!StackState.empty())
    {
        StackState.back()->update(dtime);
    }
}

void StateManager::draw(sf::RenderWindow &window)
{
    for (GameState *state : StackState)
    {
        state->draw(window);
    }
}

bool StateManager::isEmpty() const
{
    return StackState.empty();
}