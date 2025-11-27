#ifndef TREE_HPP
#define TREE_HPP

#include <SFML/Graphics.hpp>
#include "Entity.hpp"

class Tree : public Entity
{
public:
    Tree(float x, float y, float width, float height, const std::string &textureId);
    virtual ~Tree() override = default;
    void update(float dtime) override;
    Rectangle getBoundingBox() const override;

protected:
    float m_width;
    float m_height;
    sf::RectangleShape m_shape;
    sf::Sprite sp_Tree;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif