#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

struct Item {
    sf::RectangleShape rect;
    std::string name;
    int quantity = 1;
    int slotIndex = -1;
};

struct InventorySlot {
    sf::RectangleShape rect;
    Item* item = nullptr;
};

struct InventoryData {
    std::vector<Item> items;
};

