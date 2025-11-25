#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

class TextureManager
{
public:
    static TextureManager &getInstance();
    sf::Texture &getTexture(const std::string &filename);
    void loadTexture(const std::string &name, const std::string &filename);
    TextureManager(const TextureManager &) = delete;
    TextureManager &operator=(const TextureManager &) = delete;

private:
    TextureManager() = default;
    ~TextureManager() = default;
    std::map<std::string, sf::Texture> textures;
};

#endif