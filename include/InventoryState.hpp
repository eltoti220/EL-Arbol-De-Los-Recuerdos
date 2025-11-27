#ifndef INVENTORYSTATE_HPP
#define INVENTORYSTATE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "GameState.hpp"
#include "InventoryData.hpp"

class InventoryState : public GameState {
public:
    InventoryState(StateManager* manager);

    void prossesEvent(sf::Event event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow &window) override;

private:
    sf::RectangleShape background;

    InventoryData& inventory;
    std::vector<InventorySlot> slots;
    std::vector<Item> items;

    Item* grabbedItem = nullptr;
    sf::Vector2f grabOffset;

    void initSlots();
    void initItems();
};

#endif
