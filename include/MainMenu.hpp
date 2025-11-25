#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include <vector>
#include "PlayingState.hpp"
#include "TextureManager.hpp"
class MainMenu : public GameState
{
public:
    MainMenu(StateManager *_manager);
    ~MainMenu() override;

    void prossesEvent(sf::Event event) override;
    void update(float dtime) override;
    void draw(sf::RenderWindow &window) override;
    void onEntry() override;
    void onExit() override;

private:
    sf::Font font;
    std::vector<sf::Text> menuItems;
    sf::Texture txBackground;
    sf::Sprite spBackGround;
    int selectedItemIndex;

    const sf::Color NORMAL_COLOR = sf::Color::White;
    const sf::Color HOVER_COLOR = sf::Color::Magenta;

    void setupText(sf::Text &tittle, const sf::Font &font, std::string &content, float ypos, int size);
    void executeMenuItemAction(int index);
    void updateSelectionDisplay();
};
#endif