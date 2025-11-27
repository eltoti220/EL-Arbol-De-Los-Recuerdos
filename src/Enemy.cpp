#include "../include/Enemy.hpp"
#include "../include/TextureManager.hpp"
#include "../include/BTNodes.hpp"
#include <cmath>
#include <memory>

static float len(const sf::Vector2f &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

static sf::Vector2f normalize(const sf::Vector2f &v)
{
    float l = len(v);
    if (l == 0.f)
        return {0.f, 0.f};
    return {v.x / l, v.y / l};
}

Enemy::Enemy() {}

Enemy::Enemy(const sf::Vector2f &pos,
             const std::vector<sf::Vector2f> &wps,
             const std::string &textureId,
             TextureManager &textureManager)
    : position(pos), waypoints(wps), textureId(textureId)
{
    try
    {
        sprite.setTexture(textureManager.getTexture(textureId));
        sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
        sprite.setPosition(position);
        useSprite = true;
    }
    catch (...)
    {
        shape.setRadius(10.f);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(10.f, 10.f);
        shape.setPosition(position);
        useSprite = false;
    }

    if (waypoints.empty())
        waypoints.push_back(position);

    using namespace std;

    auto rootSelector = make_unique<Selector>();

    auto fleeSeq = make_unique<Sequence>();
    fleeSeq->addChild(make_unique<Cond_LowHP>(fleeHpThreshold));
    fleeSeq->addChild(make_unique<Action_Flee>());
    rootSelector->addChild(std::move(fleeSeq));

    auto chaseSeq = make_unique<Sequence>();
    chaseSeq->addChild(make_unique<Cond_PlayerInRange>(detectionRadius));
    chaseSeq->addChild(make_unique<Action_MoveToPlayer>());
    rootSelector->addChild(std::move(chaseSeq));

    rootSelector->addChild(make_unique<Action_Patrol>());

    behaviorRoot = std::move(rootSelector);
}

void Enemy::takeDamage(float d) noexcept
{
    hp -= d;
    if (hp < 0.f)
        hp = 0.f;

    if (useSprite)
        sprite.setColor(sf::Color(255, 200, 200));
    else
        shape.setFillColor(sf::Color(255, 200, 200));
}

void Enemy::moveTowards(const sf::Vector2f &target, float speed, float dt)
{
    sf::Vector2f dir = target - position;
    sf::Vector2f n = normalize(dir);
    position += n * speed * dt;

    if (useSprite)
        sprite.setPosition(position);
    else
        shape.setPosition(position);
}

void Enemy::moveAwayFrom(const sf::Vector2f &from, float speed, float dt)
{
    sf::Vector2f dir = position - from;
    sf::Vector2f n = normalize(dir);
    position += n * speed * dt;

    if (useSprite)
        sprite.setPosition(position);
    else
        shape.setPosition(position);
}

float Enemy::distanceTo(const sf::Vector2f &other) const
{
    return len(other - position);
}

void Enemy::faceTowards(const sf::Vector2f &target)
{
    if (!useSprite)
        return;
    float dx = target.x - position.x;
    if (dx < 0)
        sprite.setScale(-1.f, 1.f);
    else
        sprite.setScale(1.f, 1.f);
}

void Enemy::update(float dt, Player &player)
{
    if (!isAlive())
        return;

    // Esto ya reduce el timer y maneja que no baje de cero. ¡Correcto!
    attackTimer -= dt;
    if (attackTimer < 0.f)
        attackTimer = 0.f;

    if (behaviorRoot)
        behaviorRoot->tick(*this, player, dt);
}

void Enemy::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (useSprite)
        target.draw(sprite, states);
    else
        target.draw(shape, states);
}
