#include "InventoryState.hpp"
#include "StateManager.hpp"

InventoryState::InventoryState(StateManager* manager)
    : GameState(manager), inventory(manager->inventory)
{
    background.setSize(sf::Vector2f(500.f, 400.f));
    background.setFillColor(sf::Color(60, 60, 60, 220));
    background.setOutlineThickness(3.f);
    background.setOutlineColor(sf::Color::White);
    background.setPosition(60.f, 60.f);

    initSlots();
    initItems();
}


void InventoryState::initSlots() {
    const float slotSize = 64.f;
    const float padding = 10.f;
    const int rows = 4;
    const int cols = 4;

    float startX = 100.f;
    float startY = 110.f;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {

            InventorySlot slot;
            slot.rect.setSize(sf::Vector2f(slotSize, slotSize));
            slot.rect.setPosition(
                startX + x * (slotSize + padding),
                startY + y * (slotSize + padding)
            );

            slot.rect.setFillColor(sf::Color(80, 80, 80));
            slot.rect.setOutlineThickness(2);
            slot.rect.setOutlineColor(sf::Color(150,150,150));

            slots.push_back(slot);
        }
    }
}

void InventoryState::initItems() {
    // Si ya hay items en inventory, solo colócalos en sus slots
    for (auto& item : inventory.items) {
        if (item.slotIndex >= 0 && item.slotIndex < slots.size()) {
            slots[item.slotIndex].item = &item;

            // centrar item en slot
            sf::FloatRect b = slots[item.slotIndex].rect.getGlobalBounds();
            sf::FloatRect s = item.rect.getGlobalBounds();
            item.rect.setPosition(
                b.left + (b.width - s.width) / 2,
                b.top + (b.height - s.height) / 2
            );
        }
    }

    // Si no hay items, crea uno de ejemplo
    if (inventory.items.empty()) {
        Item item;
        item.name = "Health Potion";
        item.quantity = 1;
        item.slotIndex = 0;

        item.rect.setSize(sf::Vector2f(40, 40));
        item.rect.setFillColor(sf::Color::Green);

        inventory.items.push_back(item);
        slots[0].item = &inventory.items.back();

        // centrar
        sf::FloatRect b = slots[0].rect.getGlobalBounds();
        sf::FloatRect s = slots[0].item->rect.getGlobalBounds();
        slots[0].item->rect.setPosition(
            b.left + (b.width - s.width) / 2,
            b.top + (b.height - s.height) / 2
        );
    }
}

void InventoryState::prossesEvent(sf::Event event) {
    if (event.type == sf::Event::Closed)
        manager->getWindow().close();

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::I)
    {
        manager->popState();
    }

    // CLICK IZQUIERDO (agarra)
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mouse = manager->getWindow().mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
        );

        for (auto& slot : slots)
        {
            if (slot.item != nullptr &&
                slot.item->rect.getGlobalBounds().contains(mouse))
            {
                grabbedItem = slot.item;
                grabOffset = mouse - grabbedItem->rect.getPosition();
                slot.item = nullptr;
                break;
            }
        }
    }

    // SUELTA
if (event.type == sf::Event::MouseButtonReleased &&
    event.mouseButton.button == sf::Mouse::Left)
{
    if (grabbedItem)
    {
        sf::Vector2f mouse = manager->getWindow().mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
        );

        bool placed = false;
        for (int i = 0; i < slots.size(); ++i)
        {
            InventorySlot& slot = slots[i];

            if (slot.rect.getGlobalBounds().contains(mouse))
            {
                // quitar item del slot anterior si existía
                if (grabbedItem->slotIndex >= 0 && grabbedItem->slotIndex < slots.size())
                    slots[grabbedItem->slotIndex].item = nullptr;

                slot.item = grabbedItem;

                // centrar
                sf::FloatRect b = slot.rect.getGlobalBounds();
                sf::FloatRect s = grabbedItem->rect.getGlobalBounds();
                grabbedItem->rect.setPosition(
                    b.left + (b.width - s.width) / 2,
                    b.top + (b.height - s.height) / 2
                );

                // actualizar slotIndex
                grabbedItem->slotIndex = i;

                placed = true;
                break;
            }
        }

        // Si no lo colocó en ningún slot, volver a su slot original
        if (!placed && grabbedItem->slotIndex >= 0 && grabbedItem->slotIndex < slots.size())
        {
            InventorySlot& slot = slots[grabbedItem->slotIndex];
            slot.item = grabbedItem;

            sf::FloatRect b = slot.rect.getGlobalBounds();
            sf::FloatRect s = grabbedItem->rect.getGlobalBounds();
            grabbedItem->rect.setPosition(
                b.left + (b.width - s.width) / 2,
                b.top + (b.height - s.height) / 2
            );
        }

        grabbedItem = nullptr;
    }
 }
}

void InventoryState::update(float dt) {
    if (grabbedItem)
    {
        sf::Vector2i m = sf::Mouse::getPosition(manager->getWindow());
        sf::Vector2f worldPos = manager->getWindow().mapPixelToCoords(m);
        grabbedItem->rect.setPosition(worldPos - grabOffset);
    }
}

void InventoryState::draw(sf::RenderWindow &window) {
    window.draw(background);

    for (auto &slot : slots)
        window.draw(slot.rect);

    for (auto& item : manager->inventory.items)
        window.draw(item.rect);

    if (grabbedItem)
        window.draw(grabbedItem->rect);
}
