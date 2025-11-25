#pragma once
#include "BehaviorTree.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

class Cond_PlayerInRange : public BTNode {
public:
    Cond_PlayerInRange(float radius) : radius(radius) {}
    NodeStatus tick(Enemy& enemy, Player& player, float dt) override;

private:
    float radius;
};

class Cond_LowHP : public BTNode {
public:
    Cond_LowHP(float threshold) : threshold(threshold) {}
    NodeStatus tick(Enemy& enemy, Player&, float dt) override;
private:
    float threshold;
};


class Action_MoveToPlayer : public BTNode {
public:
    NodeStatus tick(Enemy& enemy, Player& player, float dt) override;
};

class Action_Flee : public BTNode {
public:
    NodeStatus tick(Enemy& enemy, Player& player, float dt) override;
};

class Action_Patrol : public BTNode {
public:
    NodeStatus tick(Enemy& enemy, Player&, float dt) override;
};