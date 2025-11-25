#include "../include/BTNodes.hpp"
#include <cmath>

static float len(const sf::Vector2f& v) {
    return std::sqrt(v.x*v.x + v.y*v.y);
}
static sf::Vector2f normalize(const sf::Vector2f& v) {
    float l = len(v);
    if (l == 0.f) return {0.f, 0.f};
    return {v.x / l, v.y / l};
}

NodeStatus Cond_PlayerInRange::tick(Enemy& enemy, Player& player, float dt) {
    float distance = enemy.distanceTo(player.getPosition());
    return (distance <= radius) ? NodeStatus::Success : NodeStatus::Failure;
}

NodeStatus Cond_LowHP::tick(Enemy& enemy, Player&, float dt) {
    (void)dt;
    return (enemy.getHP() <= threshold) ? NodeStatus::Success : NodeStatus::Failure;
}

NodeStatus Action_MoveToPlayer::tick(Enemy& enemy, Player& player, float dt) {
    enemy.moveTowards(player.getPosition(), enemy.getMoveSpeed(), dt);
    enemy.faceTowards(player.getPosition());
    return NodeStatus::Running;
}

NodeStatus Action_Flee::tick(Enemy& enemy, Player& player, float dt) {
    enemy.moveAwayFrom(player.getPosition(), enemy.getMoveSpeed(), dt);
    return NodeStatus::Running;
}

NodeStatus Action_Patrol::tick(Enemy& enemy, Player&, float dt) {
    sf::Vector2f target = enemy.getCurrentWaypoint();
    enemy.moveTowards(target, enemy.getMoveSpeed(), dt);

    if (enemy.distanceTo(target) < enemy.getWaypointThreshold()) {
        enemy.nextWaypoint();
    }

    return NodeStatus::Running;
}


