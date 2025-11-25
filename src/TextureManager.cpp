#include "../include/TextureManager.hpp"
#include <iostream>

TextureManager &TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

void TextureManager::loadTexture(const std::string &name, const std::string &filename)
{
    sf::Texture texture;
    if (!texture.loadFromFile(filename))
    {
        std::cerr << "Error loading texture: " << filename << std::endl;
        return;
    }
    textures.insert(std::make_pair(name, texture));
}

sf::Texture &TextureManager::getTexture(const std::string &name)
{
    auto it = textures.find(name);
    if (it != textures.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("Texture not found: " + name);
    }

    return it->second;
}
