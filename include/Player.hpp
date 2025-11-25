#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../include/MeleeAttack.hpp"
#include "../include/DodgeAbility.hpp"
#include "../include/BlockOfAbility.hpp"
#include "../include/BlockOnAbility.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Ability;

class Player : public Entity
{
public:
    Player(float startX, float startY);
    virtual ~Player() override;

    // metodos de jugabilidad
    void prossesEvent(sf::Event event);
    void update(float dtime);

    // metodos de habilidad

    // dodge
    void startDodge(float duration);
    bool isDodging() const { return m_isDodging; }
    int getCurrentHealth() const { return currentHealth; }
    void setBlocking(bool blocking) { isBloocking = blocking; }

    sf::Vector2f getPosition() const { return m_position; }

    void takeDamage(float dmg);

private:
    // atributos de combate
    int maxHealth;
    int currentHealth;
    float attackDamage;
    float attackSpeed; // este es tiempo entre ataques pilas aqui

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