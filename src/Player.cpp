#include "../include/Player.hpp"
#include <iostream>
#include <cmath>

Player::Player(float startX, float startY)
{
    m_position = sf::Vector2f(startX, startY);
    m_speed = 300.0f;
    currentMovement = sf::Vector2f(0.0f, 0.0f);

    maxHealth = 100;
    currentHealth = maxHealth;
    attackDamage = 15.0f;
    attackSpeed = 0.8f;

    isBloocking = false;
    m_isDodging = false;
    dodgeTime = 0.0f;
    dodgeDirection = {0.0f, 0.0f};
    m_dodgeSpeed = 600.0f;

    playerShape.setRadius(25.0f);
    playerShape.setFillColor(sf::Color::Blue);
    playerShape.setOrigin(25.0f, 25.0f);
    playerShape.setPosition(m_position);

    meeleAttack = new MeeleAttack;
    dodgeAbility = new DodgeAbility;
    blockOfAbility = new BlockOfAbility;
    blockOnAbility = new BlockOnAbility;
}

Player::~Player()
{
    delete meeleAttack;
    delete dodgeAbility;
    delete blockOfAbility;
    delete blockOnAbility;
}

Rectangle Player::getBoundingBox() const
{
    float radius = playerShape.getRadius();
    sf::Vector2f pos = playerShape.getPosition();
    return Rectangle(pos.x - radius, pos.y - radius, radius * 2, radius * 2);
}

void Player::prossesEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::K)
        {
            if (meeleAttack->isReady())
            {
                meeleAttack->execute(this);
            }
        }
        else if (event.key.code == sf::Keyboard::J)
        {
            if (dodgeAbility->isReady())
            {
                dodgeAbility->execute(this);
            }
        }
        else if (event.key.code == sf::Keyboard::LShift)
        {
            std::cout << "DEBUG: Bloqueo iniciado (BlockOnAbility).\n";
            playerShape.setFillColor(sf::Color::Green);
            blockOnAbility->execute(this);
        }

        if (event.key.code == sf::Keyboard::W)
        {
            currentMovement.y = -1.0f;
        }
        else if (event.key.code == sf::Keyboard::S)
        {
            currentMovement.y = 1.0f;
        }
        else if (event.key.code == sf::Keyboard::A)
        {
            currentMovement.x = -1.0f;
        }
        else if (event.key.code == sf::Keyboard::D)
        {
            currentMovement.x = 1.0f;
        }
    }

    else if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::LShift)
        {
            std::cout << "DEBUG: Bloqueo Finalizada (BlockOfAbility).\n";
            playerShape.setFillColor(sf::Color::Blue);
            blockOfAbility->execute(this);
        }
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
        {
            currentMovement.y = 0.0f;
        }
        else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
        {
            currentMovement.x = 0.0f;
        }
    }
}

void Player::startDodge(float duration)
{
    if (m_isDodging)
    {
        return;
    }

    dodgeTime = duration;
    m_isDodging = true;

    dodgeDirection = currentMovement;

    if (currentMovement.x == 0.0f && currentMovement.y == 0.0f)
    {
        dodgeDirection.y = 1.0f;
        ;
    }

    playerShape.setFillColor(sf::Color::Cyan);
    std::cout << "DEBUG: Jugador ha iniciado esquiva por " << duration << " segundos.\n";
}

void Player::update(float dtime)
{
    meeleAttack->update(dtime);
    dodgeAbility->update(dtime);

    sf::Vector2f movement = currentMovement;

    if (m_isDodging)
    {
        dodgeTime -= dtime;
        if (dodgeTime > 0.0f)
        {
            sf::Vector2f displacement = dodgeDirection * m_dodgeSpeed * dtime;
            m_position += displacement;
        }
        else
        {
            m_isDodging = false;
            playerShape.setFillColor(sf::Color::Blue);
            std::cout << "DEBUG: Jugador ha terminado la esquiva.\n";
        }
    }

    float effectiveSpeed = m_speed;
    const float BLOCKING_SPEED_PENALTY = 0.5f;
    if (isBloocking)
    {
        effectiveSpeed = m_speed * BLOCKING_SPEED_PENALTY;
    }

    if (!m_isDodging)
    {
        if (movement.x != 0.0f || movement.y != 0.0f)
        {
            float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= length;
        }

        m_position += movement * effectiveSpeed * dtime;
    }

    playerShape.setPosition(m_position);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(playerShape, states);
}
