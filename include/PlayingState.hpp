#ifndef PLAYINGSTATE_HPP
#define PLAYINGSTATE_HPP

#include "GameState.hpp"
#include "StateManager.hpp"
#include "EntityManager.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>
class PlayingState : public GameState
{
public:
    PlayingState(StateManager *manager);
    virtual ~PlayingState() override;
    void prossesEvent(sf::Event event) override;
    void update(float dtime) override;
    void draw(sf::RenderWindow &window) override;
    void onEntry() override;
    void onExit() override;

    PlayingState(const PlayingState &) = delete;
    PlayingState &operator=(const PlayingState &) = delete;
    PlayingState(PlayingState &&) = delete;
    PlayingState &operator=(PlayingState &&) = delete;

private:
    EntityManager *m_entityManager;
    sf::View m_gameView;
    Player *m_player;
    sf::Sprite sp_background;
    void handleCamera(float dtime);
};

#endif