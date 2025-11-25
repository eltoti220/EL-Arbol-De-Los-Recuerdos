#include "../include/MainMenu.hpp"
#include "../include/StateManager.hpp"

MainMenu::MainMenu(StateManager *_manager) : GameState(_manager)
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::setupText(sf::Text &tittle, const sf::Font &font, std::string &content, float ypos, int size)
{
    tittle.setFont(font);
    tittle.setFillColor(NORMAL_COLOR);
    tittle.setString(content);
    tittle.setCharacterSize(size);

    sf::FloatRect bounds = tittle.getLocalBounds();
    tittle.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    tittle.setPosition(205.0f, ypos);
}

void MainMenu::executeMenuItemAction(int index)
{
    switch (index)
    {
    case 0:
        std::cout << "DEBUG: Continuar seleccionado. cambiando estado";
        manager->changeState(new PlayingState(manager));
        break;
    case 1:
        std::cout << "DEBUG: Nuevo Juego seleccionado. cambiando estado";
        break;
    case 2:
        std::cout << "DEBUG: Opciones seleccionado. cambiando estado";
        break;
    }
}

void MainMenu::onEntry()
{
    std::cout << "DEBUG: Entrando al MainMenuState. Cargando recursos...\n";
    if (!font.loadFromFile("assets/fonts/Jersey10-Regular.ttf"))
    {
        std::cout << "the font is not here";
    }
    std::cout << "the font is here" << std::endl;
    txBackground = TextureManager::getInstance().getTexture("Menu_Background");
    std::cout << "the background image is here" << std::endl;
    spBackGround.setTexture(txBackground);
    sf::Vector2u windowSize = manager->getWindow().getSize();
    sf::Vector2u textureSize = txBackground.getSize();

    float scaleX = (float)windowSize.x / textureSize.x;
    float scaley = (float)windowSize.y / textureSize.y;
    spBackGround.setScale(scaleX, scaley);

    std::vector<std::string> nameItems = {"Continuar", "Nueva Partida", "Opciones"};
    float ypos = 110;
    for (size_t i = 0; i < 3; i++)
    {
        sf::Text item;
        setupText(item, font, nameItems[i], ypos, 35);
        menuItems.push_back(item);
        ypos += 85;
    }
    selectedItemIndex = 0;
}

void MainMenu::onExit()
{
    std::cout << "DEBUG: Saliendo del MainMenuState.\n";
}

void MainMenu::prossesEvent(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousepos = manager->getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        for (size_t i = 0; i < menuItems.size(); i++)
        {
            sf::FloatRect bounds = menuItems[i].getGlobalBounds();

            if (bounds.contains(mousepos))
            {
                executeMenuItemAction(i);
                break;
            }
        }
    }
}

void MainMenu::update(float dtime)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(manager->getWindow());
    sf::Vector2f wordlPos = manager->getWindow().mapPixelToCoords(pixelPos);
    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::FloatRect bounds = menuItems[i].getGlobalBounds();

        if (bounds.contains(wordlPos))
        {
            selectedItemIndex = i;
            break;
        }
    }

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        if (i == selectedItemIndex)
        {
            menuItems[i].setFillColor(HOVER_COLOR);
        }
        else
        {
            menuItems[i].setFillColor(NORMAL_COLOR);
        }
    }
}

void MainMenu::draw(sf::RenderWindow &window)
{
    window.draw(spBackGround);
    for (const auto &item : menuItems)
    {
        window.draw(item);
    }
}
