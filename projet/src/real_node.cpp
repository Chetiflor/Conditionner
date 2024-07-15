#include <algorithm>

#include "node.hpp"

RealNode::RealNode()
{

    heritage.patterns_mutability = 0.1;
    heritage.mutability_mutability = 0.1;
    heritage.learning_strength_mutability = 0.1;
    heritage.death_trigger_up_mutability = 0.1;
    heritage.death_trigger_down_mutability = 0.1;
    heritage.duplicate_trigger_mutability = 0.1;
    heritage.temperature_change_high_mutability = 0.1;
    heritage.temperature_change_low_mutability = 0.1;
    heritage.temperature_up_mutability = 0.1;
    heritage.temperature_divider_mutability = 0.1;
    heritage.probabilities_resistance_mutability = 0.1;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine generator(seed);
    std::normal_distribution<float> distribution(0, 1);
    heritage.mutability = 1;
    heritage.death_trigger_down = abs(distribution(generator));
    heritage.death_trigger_up = heritage.death_trigger_down + abs(distribution(generator)) + 1;
    heritage.duplicate_trigger = heritage.death_trigger_down + abs(distribution(generator));
    heritage.learning_strength = (heritage.death_trigger_down + heritage.death_trigger_up) / 2;
    heritage.temperature_change_high = abs(distribution(generator));
    heritage.temperature_change_low = abs(distribution(generator));
    heritage.temperature_up = abs(distribution(generator));
    heritage.temperature_divider = exp(distribution(generator)) + 1;
    heritage.probabilities_resistance = exp(distribution(generator)) + 1;

    float general_trig = 0;
    float trig_patterns = 1;

    while (trig_patterns > general_trig)
    {
        absolute_pattern p;
        float trig_indices = 1;
        while (trig_indices > general_trig)
        {

            std::string index = "";
            float trig_sides = 1;
            while (trig_sides > general_trig)
            {
                if (distribution(generator) > general_trig)
                {
                    index += "1";
                }
                else
                {
                    index += "0";
                }
                trig_sides = distribution(generator);
            }
            p.components.push_back(TreeIndex(index));
            trig_indices = distribution(generator);
        }

        float sum = 0;
        for (int k = 0; k < number_of_actions; k++)
        {
            float tmp = abs(distribution(generator));
            sum += tmp;
            p.probabilities.push_back(tmp);
        }

        for (int k = 0; k < number_of_actions; k++)
        {
            p.probabilities[k] /= sum;
        }

        heritage.patterns.push_back(p);

        trig_patterns = distribution(generator);
    }
    main_tree->birthing_nodes.push_back(this);
    state = down;
}

RealNode::~RealNode()
{
}

RealNode::RealNode(Inheritance h)
{
    heritage = h;
    main_tree->birthing_nodes.push_back(this);
    state = down;
}

void RealNode::onBirth()
{
    // get all different nodes appearing in patterns
    int local_id = 0;
    std::map<TreeIndex, int> tmp_index_to_int;
    for (int i = 0; i < heritage.patterns.size(); i++)
    {
        absolute_pattern p = heritage.patterns[i];
        for (int j = 0; j < p.components.size(); j++)
        {
            TreeIndex tree_id = p.components[j];
            if (tmp_index_to_int.find(tree_id) == tmp_index_to_int.end())
            {
                tmp_index_to_int.insert({tree_id, local_id});
                subscribed.push_back(tree_id);
                local_id++;
            }
        }
        local_pattern transcribed_p;
        transcribed_p.probabilities = p.probabilities;
        transcribed_p.components = p.components;
        transcribed_p.frequencies = std::vector<float>(number_of_actions, 0);
        patterns.push_back(transcribed_p);
    }
}

void RealNode::fetch()
{
    std::vector<TreeIndex> nodes_seen_activated;
    for (int i = 0; i < subscribed.size(); i++)
    {
        Node *subscribed_node = main_tree->getNode(subscribed[i]);
        if (subscribed_node->getState())
        {
            nodes_seen_activated.push_back(subscribed[i]);
        }
    }
    if (nodes_seen_activated.size() > 0)
    {
        short_memory.push_front(nodes_seen_activated);
    }
    if (short_memory.size() > max_short_memory_size)
    {
        short_memory.pop_back();
    }
}

void RealNode::refresh()
{
    // watch subscribed nodes, gather activated, store in short memory
    // then watch if some patterns are triggered
    // if so, get an action from it, collect all actions, then process them

    std::vector<bool> found_patterns = look_if_in_memory(patterns, short_memory);
    std::vector<int> actions_count = std::vector<int>(number_of_actions, 0);
    for (int i = 0; i < patterns.size(); i++)
    {
        if (found_patterns[i])
        {
            Action a = getAction(patterns[i]);
            actions_count[a]++;
            patterns[i].frequencies[a] += temperature;
        }
        for (int j = 0; j < number_of_actions; j++)
        {
            patterns[i].frequencies[j] /= heritage.probabilities_resistance;
        }
    }
    std::vector<Action> to_do = desambiguate_action(actions_count);

    std::deque<std::vector<TreeIndex>> tmp_short = short_memory;
    std::deque<std::vector<TreeIndex>> tmp_long = long_memory;
    bool to_load = false;
    for (int i = 0; i < to_do.size(); i++)
    {
        switch (to_do[i])
        {
        case High:
            next_state = up;
            break;
        case Low:
            next_state = down;
            break;
        case Switch:
            next_state = !state;
            break;
        case Flush:
            short_memory = {};
            break;
        case Load:
            to_load = true;
        case Store:
            long_memory = tmp_short;
        case Refine:
            refine_subscribed();
        default:
            break;
        }
    }
    if (next_state != state)
    {
        temperature += next_state ? heritage.temperature_change_high : heritage.temperature_change_low;
    }
    if (next_state)
    {
        temperature += heritage.temperature_up;
    }
    temperature /= heritage.temperature_divider;
    if (to_load)
    {
        short_memory = tmp_long;
    }
}

void RealNode::step()
{
    state = next_state;
}

void RealNode::learn(float commander_temperature)
{
    for (int i = 0; i < patterns.size(); i++)
    {
        adjustProbas(patterns[i].probabilities, patterns[i].frequencies, temperature, commander_temperature, heritage.learning_strength);
    }
}

void RealNode::refine_subscribed()
{
    // command subscribed nodes to learn
    for (int i = 0; i < subscribed.size(); i++)
    {
        Node *subscribed_node = main_tree->getNode(subscribed[i]);
        subscribed_node->learn(temperature);
    }
}

activation RealNode::getState()
{
    return state;
}

void RealNode::displace(TreeIndex origin, TreeIndex destination)
{

    Inheritance mutated_heritage = heritage;
    mutated_heritage.move(origin, destination);
}

Node *RealNode::copy(bool mutations)
{
    Inheritance h = heritage;
    if (mutations)
    {
        heritage.mutate();
    }
    return new RealNode(heritage);
}

NodeType RealNode::type()
{
    return Real;
}

Node *RealNode::getInstance()
{
    return this;
}

std::string RealNode::toString()
{
    return std::to_string(getState());
}
