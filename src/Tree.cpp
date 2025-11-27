#include "../include/Tree.hpp"
#include "../include/TextureManager.hpp"
#include <iostream>

Tree::Tree(float x, float y, float width, float height, const std::string &textureId)
    : m_width(width), m_height(height)
{
    m_position = sf::Vector2f(x, y);
    m_shape.setSize(sf::Vector2f(width, height));
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setPosition(m_position);

    try
    {
        sf::Texture &texture = TextureManager::getInstance().getTexture(textureId);
        sp_Tree.setTexture(texture);
        texture.setRepeated(true);
        sp_Tree.setTextureRect(sf::IntRect(0, 0, static_cast<int>(width), static_cast<int>(height)));
        sp_Tree.setPosition(m_position);
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: Textura del arbol/obstaculo no encontrada: " << textureId << std::endl;
    }
}

Rectangle Tree::getBoundingBox() const
{
    sf::Vector2f pos = m_shape.getPosition();
    sf::Vector2f size = m_shape.getSize();
    return Rectangle(pos.x, pos.y, size.x, size.y);
}

void Tree::update(float dtime)
{
    // son estaticos, no hacen nada en update
}

void Tree::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(sp_Tree, states);

    // 2. DEBUG: Dibujar elHITBOX real (el pequeño y acomodado)
    if (true)
    {
        Rectangle rect = getBoundingBox();
        sf::RectangleShape boundsShape;
        boundsShape.setSize(sf::Vector2f(rect.width, rect.height));
        boundsShape.setPosition(rect.x, rect.y);
        boundsShape.setFillColor(sf::Color(255, 0, 0, 80)); // Rojo semi-transparente
        boundsShape.setOutlineThickness(1.0f);
        boundsShape.setOutlineColor(sf::Color::Red);
        target.draw(boundsShape, states);
    }
}
