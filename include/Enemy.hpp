#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "BehaviorTree.hpp"
#include "Entity.hpp"
#include "TextureManager.hpp"

class Player;
class BTNode;

class Enemy : public Entity
{
public:
      Enemy();
      Enemy(const sf::Vector2f &pos,
            const std::vector<sf::Vector2f> &waypoints,
            const std::string &textureId,
            TextureManager &textureManager);

      void update(float dt, Player &player);
      virtual void update(float dt) override {}
      virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

      sf::Vector2f getPosition() const noexcept { return position; }
      void setPosition(const sf::Vector2f &p)
      {
            position = p;
            shape.setPosition(p);
      }

      float getHP() const noexcept { return hp; }
      float getMaxHP() const noexcept { return maxHp; }
      void takeDamage(float d) noexcept;
      bool isAlive() const noexcept { return hp > 0.f; }

      void moveTowards(const sf::Vector2f &target, float speed, float dt);
      void moveAwayFrom(const sf::Vector2f &from, float speed, float dt);

      float distanceTo(const sf::Vector2f &other) const;
      void faceTowards(const sf::Vector2f &target);

      float getMoveSpeed() const { return moveSpeed; }
      const std::vector<sf::Vector2f> &getWaypoints() const { return waypoints; }
      sf::Vector2f getPositionRaw() const { return position; }

      const sf::Vector2f &getCurrentWaypoint() const { return waypoints[currentWaypoint]; }
      float getWaypointThreshold() const { return waypointReachThreshold; }
      void nextWaypoint() { currentWaypoint = (currentWaypoint + 1) % waypoints.size(); }
      Rectangle getBoundingBox() const override
      {
            float radius = useSprite ? sprite.getGlobalBounds().width / 2.f : shape.getRadius();
            return Rectangle(position.x - radius, position.y - radius, radius * 2, radius * 2);
      }

private:
      sf::CircleShape shape;
      sf::Sprite sprite;
      bool useSprite = false;

      sf::Vector2f position;

      float hp = 100.f;
      float maxHp = 100.f;
      float moveSpeed = 100.f;
      float chaseSpeed = 700.f;
      float detectionRadius = 600.f;
      float attackRadius = 32.f;
      float fleeHpThreshold = 25.f;

      float attackCooldown = 1.0f;
      float attackTimer = 0.f;

      std::vector<sf::Vector2f> waypoints;
      size_t currentWaypoint = 0;
      float waypointReachThreshold = 8.f;

      std::unique_ptr<BTNode> behaviorRoot;

      std::string textureId;
};
