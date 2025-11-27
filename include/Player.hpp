#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../include/MeleeAttack.hpp"
#include "../include/DodgeAbility.hpp"
#include "../include/BlockOfAbility.hpp"
#include "../include/BlockOnAbility.hpp"
#include "../include/Rectangle.hpp"
#include "InventoryData.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Ability;

class Player : public Entity
{
public:
    Player(float startX, float startY);
    virtual ~Player() override;

    // Rectangle para colisiones
    Rectangle getBoundingBox() const override;

    // Métodos de jugabilidad
    void prossesEvent(sf::Event event);
    void update(float dtime);

    // Métodos de habilidad
    void startDodge(float duration);
    bool isDodging() const { return m_isDodging; }

    // Vida
    int getCurrentHealth() const { return currentHealth; }
    int getMaxHealth() const { return maxHealth; }  // getter para maxHealth
    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive() const { return currentHealth > 0; }

    // Block
    bool isBlocking() const { return isBlockingFlag; }
    void setBlocking(bool blocking) { isBlockingFlag = blocking; }

    // Movimiento
    sf::Vector2f getCurrentMovement() const { return currentMovement; }

    // Ataque
    Ability *getMeeleAttack() const { return meeleAttack; }
    float getAttackDamage() const { return attackDamage; }
    float getAttackRange() const { return rangeAttack; }

    // Draw público para poder llamarlo desde otros estados
    virtual void draw(sf::RenderTarget &Target, sf::RenderStates states) const override;
    
    InventoryData& getInventory() { return inventory; }

    // Métodos para obtener información de la vida (útil para dibujar la barra)
    int getCurrentHP() const { return currentHP; }
    int getMaxHP() const { return maxHP; }

private:
    // Atributos de combate
    int maxHealth;
    int currentHealth;
    float attackDamage;
    float attackSpeed; // Tiempo entre ataques
    float rangeAttack;

    // Colisión
    sf::Vector2f currentPosition;

    // Atributos de habilidad
    bool isBlockingFlag;

    // Atributos para el esquive
    bool m_isDodging;
    float dodgeTime;
    sf::Vector2f dodgeDirection;
    float m_dodgeSpeed = 600.0f;

    // Ataques
    Ability *meeleAttack;
    Ability *dodgeAbility;
    Ability *blockOnAbility;
    Ability *blockOfAbility;

    // Propiedades visuales y de movimiento
    sf::CircleShape playerShape;
    sf::Vector2f currentMovement;

    InventoryData inventory;

    int currentHP; // Puntos de vida actuales
    const int maxHP = 100; // Puntos de vida máximos
};

#endif
