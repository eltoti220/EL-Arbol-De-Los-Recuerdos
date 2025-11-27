#pragma once
#include <vector>
#include <memory>

enum class NodeStatus {
    Success,
    Failure,
    Running
};

class Enemy;
class Player;

class BTNode {
public:
    virtual ~BTNode() = default;
    virtual NodeStatus tick(Enemy& enemy, Player& player, float dt) = 0;
};

class CompositeNode : public BTNode {
public:
    void addChild(std::unique_ptr<BTNode> child) {
        children.push_back(std::move(child));
    }
protected:
    std::vector<std::unique_ptr<BTNode>> children;
};

class Selector : public CompositeNode {
public:
    NodeStatus tick(Enemy& enemy, Player& player, float dt) override {
        for (size_t i = 0; i < children.size(); ++i) {
            NodeStatus st = children[i]->tick(enemy, player, dt);
            if (st == NodeStatus::Success) return NodeStatus::Success;
            if (st == NodeStatus::Running) return NodeStatus::Running;
        }
        return NodeStatus::Failure;
    }
};


class Sequence : public CompositeNode {
public:
    NodeStatus tick(Enemy& enemy, Player& player, float dt) override {
        for (size_t i = 0; i < children.size(); ++i) {
            NodeStatus st = children[i]->tick(enemy, player, dt);
            if (st == NodeStatus::Failure) return NodeStatus::Failure;
            if (st == NodeStatus::Running) return NodeStatus::Running;
        }
        return NodeStatus::Success;
    }
};
