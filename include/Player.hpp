#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../include/MeleeAttack.hpp"
#include "../include/DodgeAbility.hpp"
#include "BlockOfAbility.hpp"
#include "BlockOnAbility.hpp"
#include "Rectangle.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameMap.hpp"

class Ability;

class Player : public Entity
{
public:
    Player(float startX, float startY);
    virtual ~Player() override;

    // rectangle para colisiones
    Rectangle getBoundingBox() const override;
    void checkMapCollision(GameMap &gameMap);

    // metodos de jugabilidad
    void prossesEvent(sf::Event event);
    void update(float dtime);

    // metodos de habilidad

    // dodge
    void startDodge(float duration);
    bool isDodging() const { return m_isDodging; }

    // vida
    int getCurrentHealth() const { return currentHealth; }
    // block
    bool isBlocking() const { return isBloocking; }
    void setBlocking(bool blocking) { isBloocking = blocking; }
    // movimiento
    sf::Vector2f getCurrentMovement() const { return currentMovement; }

    // ataque
    Ability *getMeeleAttack() const { return meeleAttack; }
    float getAttackDamage() const { return attackDamage; }
    float getAttackrange() const { return rangeAttack; }

private:
    // atributos de combate
    int maxHealth;
    int currentHealth;
    float attackDamage;
    float attackSpeed; // este es tiempo entre ataques pilas aqui
    float rangeAttack;

    // Colision
    sf::Vector2f currentPosition;
    ;

    // atributos de habilidad
    bool isBloocking;

    // atributos para el esquive
    bool m_isDodging;
    float dodgeTime;
    sf::Vector2f dodgeDirection;
    float m_dodgeSpeed = 600.0f;

    // ataques
    Ability *meeleAttack;
    Ability *dodgeAbility;
    Ability *blockOnAbility;
    Ability *blockOfAbility;

    //! propiedades visuales y de movimiento de momento
    sf::CircleShape playerShape;
    sf::Vector2f currentMovement;

    virtual void draw(sf::RenderTarget &Target, sf::RenderStates states) const override;
};

#endif