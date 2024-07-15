#include <node.hpp>

std::vector<bool> look_if_in_memory(std::vector<local_pattern> to_find, std::deque<std::vector<TreeIndex>> memory)
{
    // search if any pattern in list is in short memory
    // a pattern is considered as found if its nodes appear in the right order in memory
    // no matter how far from each other
    std::vector<int> i_patterns(to_find.size(), 0);
    std::vector<bool> patterns_found(to_find.size(), false);

    for (int i_memory = 0; i_memory < memory.size(); i_memory++)
    {
        for (int k = 0; k < to_find.size(); k++)
        {
            if (!patterns_found[k])
            {
                TreeIndex to_find_index = to_find[k].components[i_patterns[k]];
                bool int_found = false;
                for (int n = 0; n < memory[i_memory].size(); n++)
                {
                    int_found |= to_find_index == memory[i_memory][n];
                }
                if (int_found)
                {
                    i_patterns[k]++;
                    patterns_found[k] = i_patterns[k] == to_find[k].components.size();
                }
            }
        }
    }
    return patterns_found;
}

Action getAction(local_pattern p)
{
    // from a given pattern, with a given probability distribution, choose randomly an action
    std::uniform_real_distribution<float> distribution(0, 1);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine generator(seed);
    float x = distribution(generator);
    int i = 0;
    float sum = p.probabilities[i];
    while (x > sum)
    {
        i++;
        sum += p.probabilities[i];
    }
    return int_to_action[i];
}

std::vector<Action> desambiguate_action(std::vector<int> actions_count)
{
    // from a vector of actions, got by a node after refreshing, sort and remove incompatibilities
    std::vector<Action> to_do;
    for (int i = 3; i < number_of_actions; i++)
    {
        if (actions_count[i])
        {
            to_do.push_back(int_to_action[i]);
        }
    }
    if (actions_count[High] == 0 && actions_count[Low] == 0)
    {
        if (actions_count[Switch] != 0)
        {
            to_do.push_back(Switch);
        }
    }
    else
    {
        if (actions_count[High] > actions_count[Low])
        {
            to_do.push_back(High);
        }
        else if (actions_count[High] < actions_count[Low])
        {
            to_do.push_back(Low);
        }
        else
        {
            to_do.push_back(Switch);
        }
    }
    return to_do;
};

void adjustProbas(std::vector<float> &probas, std::vector<float> action_frequency, float self_temperature, float commander_temperature, float learning_strength)
{
    // reajust probabilities distribution of actions from a given node followinig these patterns:
    //      - hotness of the given node will ensure low randomness :
    //          if the node is used a lot, it should be stable
    //      - exceptionnality of an action will be rewarded : 
    //          it meant this action had been able to trigger another node to feedback on itself
    //          thus promoting synergy through complex retroactions
    //      - commander hotness will ensure lower impact of retroaction:
    //          scarcity of firing would be correlated with higher value of reward
    //      - learning strength will ensure higher impact of retroaction
    //           node specific parameter to determine behavior regarding learning
    float sum = 0;
    std::vector<float> delta_probas(probas.size(), 0);
    std::uniform_real_distribution<float> distribution(0, 1 / self_temperature);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine generator(seed);
    for (int i = 0; i < delta_probas.size(); i++)
    {
        delta_probas[i] = exp(action_frequency[i]/probas[i] + distribution(generator));
        sum += probas[i];
    }
    float weight = learning_strength/commander_temperature;
    for (int i = 0; i < delta_probas.size(); i++)
    {
        probas[i] = probas[i] + weight*delta_probas[i];
        probas[i] /= (1 + weight);
    }
};