#ifndef NODE_HPP
#define NODE_HPP
#define up true
#define down false
#define activation bool
#define all_actions High, Low, Switch, Flush, Store, Load, Refine

#include <map>
#include <deque>

#include <random>
#include <chrono>

#include "tree_index.hpp"
#include "tree.tpp"

const int max_short_memory_size = 10;

enum Action
{
    all_actions,
    number_of_actions
};

enum NodeType;

const std::vector<Action> int_to_action = {all_actions};

struct absolute_pattern
{
    std::vector<TreeIndex> components;
    std::vector<float> probabilities;
};
struct local_pattern
{
    std::vector<TreeIndex> components;
    std::vector<float> probabilities;
    std::vector<float> frequencies;
};

std::vector<bool> look_if_in_memory(std::vector<local_pattern> to_find, std::deque<std::vector<TreeIndex>> memory);

Action getAction(local_pattern p);

std::vector<Action> desambiguate_action(std::vector<int> Action_count);

void adjustProbas(std::vector<float> &probas, std::vector<float> action_frequency, float self_temperature, float commander_temperature, float learning_strength);

class Inheritance
{
public:
    std::vector<absolute_pattern> patterns;

    float patterns_mutability;
    float mutability;
    float mutability_mutability;
    float learning_strength;
    float learning_strength_mutability;
    float death_trigger_up;
    float death_trigger_up_mutability;
    float death_trigger_down;
    float death_trigger_down_mutability;
    float duplicate_trigger;
    float duplicate_trigger_mutability;
    float temperature_change_high;
    float temperature_change_high_mutability;
    float temperature_change_low;
    float temperature_change_low_mutability;
    float temperature_up;
    float temperature_up_mutability;
    float temperature_divider;
    float temperature_divider_mutability;
    float probabilities_resistance;
    float probabilities_resistance_mutability;

    void move(TreeIndex former, TreeIndex later);
    void mutate();
};

class Node
{
protected:
    Node(){};

    bool dying_flag = false;

public:
    static Tree<Node> *const main_tree;

    virtual ~Node(){};



    virtual void setFlag(bool b) = 0;
    virtual void onBirth() = 0;
    virtual void refresh() = 0;
    virtual void step() = 0;
    virtual void learn(float commander_temperature) = 0;
    virtual activation getState() = 0;
    virtual void displace(TreeIndex origin, TreeIndex destination) = 0;
    virtual Node *copy(bool mutations) = 0;
    virtual NodeType type() = 0;
    virtual Node *getInstance() = 0;
    virtual std::string toString() = 0;

    bool isDying(){return dying_flag;}
    Node *getAbstract(TreeIndex linked);
};

class AbstractNode : public Node
{
private:
    TreeIndex linked_index;
    Node * instance = nullptr;
    float left_proba = 0;

public:
    AbstractNode();
    AbstractNode(TreeIndex linked_index);
    ~AbstractNode();

    void setFlag(bool b){dying_flag = false;};
    void onBirth();
    void refresh();
    void step();
    void learn(float commander_temperature);
    activation getState();
    void displace(TreeIndex origin, TreeIndex destination);
    Node *copy(bool mutations);
    NodeType type();
    Node *getInstance();
    std::string toString();
};

class RealNode : public Node
{
private:
    Inheritance heritage;
    std::vector<TreeIndex> subscribed;

    std::deque<std::vector<TreeIndex>> short_memory;
    std::deque<std::vector<TreeIndex>> long_memory;
    std::vector<local_pattern> patterns;

    float temperature;
    activation state;

    activation next_state;

    void fetch();
    void refine_subscribed();

public:
    RealNode();
    RealNode(Inheritance h);
    ~RealNode();

    void setFlag(bool b){dying_flag = b;};
    void onBirth();
    void refresh();
    void step();
    void learn(float commander_temperature);
    activation getState();
    void displace(TreeIndex origin, TreeIndex destination);
    Node *copy(bool mutations);
    NodeType type();
    Node *getInstance();
    std::string toString();
};

#endif