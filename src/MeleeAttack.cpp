#include "../include/MeleeAttack.hpp"
#include "../include/Quadtree.hpp"
#include <iostream>

MeeleAttack::MeeleAttack()
{
    cooldown = 1.0f;
    cooldownTimer = 0.0f;
    secuenceStep = 0;
    secuenceStepTimer = 0.0;
}

float MeeleAttack::getDamageForStep(int step) const
{
    if (step == 1)
    {
        return 10.0f;
    }
    if (step == 2)
    {
        return 10.0f;
    }
    if (step == 3)
    {
        return 20.0f;
    }
    return 0.0;
}

Rectangle MeeleAttack::calculateHitbox(sf::Vector2f ownerPosition, sf::Vector2f direction, int step) const
{ //! Acomodar bien las dimensiones y posiciones de la hitbox segun el paso del ataque (la direccion del ataque se asume siempre a la derecha por ahora)
    const float PLAYER_RADIUS = 25.0f;
    float attackDistance = 5.0f;

    float baseWidth = 40.0f;
    float baseHeight = 60.0f;

    if (step == 3)
    {
        baseWidth = 60.0f;
        baseHeight = 80.0f;
        attackDistance = 8.0f;
    }

    float dirLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (dirLength == 0.0f)
    {
        direction = sf::Vector2f(1.0f, 0.0f);
        dirLength = 1.0f;
    }
    else
    {
        direction.x /= dirLength;
        direction.y /= dirLength;
    }

    bool isVertical = std::abs(direction.y) > std::abs(direction.x);
    float finalWidth, finalHeight;

    if (isVertical)
    {
        finalWidth = baseHeight;
        finalHeight = baseWidth;
    }
    else
    {
        finalWidth = baseWidth;
        finalHeight = baseHeight;
    }

    sf::Vector2f attacCenter;

    float totalDistance = attackDistance + (isVertical ? finalHeight : finalWidth) / 2.0f + PLAYER_RADIUS;
    attacCenter.x = ownerPosition.x + direction.x * totalDistance;
    attacCenter.y = ownerPosition.y + direction.y * totalDistance;

    float hitboxX = attacCenter.x - finalWidth / 2.0f;
    float hitboxY = attacCenter.y - finalHeight / 2.0f;

    return Rectangle(hitboxX, hitboxY, finalWidth, finalHeight);
}

void MeeleAttack::execute(Entity *owner, Quadtree *collisionTree)
{
    if (!isReady())
    {
        return;
    }

    if (secuenceStepTimer > 0.0f)
    {
        secuenceStep++;
    }
    else
    {
        secuenceStep = 1;
    }

    std::cout << "DEBUG: Ataque Melee #" << secuenceStep << " ejecutado. Daño: "
              << getDamageForStep(secuenceStep) << " | Posición: "
              << owner->getPosition().x << std::endl;

    Player *player = dynamic_cast<Player *>(owner);
    if (!player)
    {
        std::cout << "ERROR: El dueño del ataque melee no es un jugador válido.\n";
        return;
    }
    sf::Vector2f direction = player->getCurrentMovement();

    if (collisionTree)
    {
        Rectangle hitRange = calculateHitbox(owner->getPosition(), direction, secuenceStep);
        std::vector<Entity *> candidates = collisionTree->query(hitRange);
        for (Entity *entity : candidates)
        {
            std::cout << "DEBUG: Entidad golpeada en el ataque melee en la posición ("
                      << entity->getPosition().x << ", " << entity->getPosition().y << ")\n";

            if (entity == owner)
            {
                continue;
            }

            if (hitRange.intersects(entity->getBoundingBox()))
            {
                std::cout << "DEBUG: Entidad afectada por el ataque melee. Daño recibido: "
                          << getDamageForStep(secuenceStep) << "\n";
                // Aquí se aplicaría el daño a la entidad
                // targetEntity->applyDamage(getDamageForStep(secuenceStep));
            }
        }
    }

    if (secuenceStep == 3)
    {

        cooldownTimer = COOLDOWN_SPECIAL;
        secuenceStep = 0;
        secuenceStepTimer = 0.0f;
        std::cout << "DEBUG: secuencia de ataque reiniciada" << std::endl;
    }
    else
    {
        cooldownTimer = COOLDOWN_NORMAL;
        secuenceStepTimer = 0.69f;
    }
}

void MeeleAttack::update(float dtime)
{
    if (cooldownTimer > 0.0f)
    {
        cooldownTimer -= dtime;
    }

    if (secuenceStepTimer > 0.0f)
    {
        secuenceStepTimer -= dtime;
        if (secuenceStepTimer <= 0.0f)
        {
            std::cout << "DEBUG: secuencia de ataque reiniciada" << std::endl;
            secuenceStep = 0;
        }
    }
}

void MeeleAttack::draw(sf::RenderWindow &window)
{
    // se dibujara el ataque
}
