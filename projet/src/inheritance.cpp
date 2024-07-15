#include "node.hpp"

void Inheritance::move(TreeIndex former, TreeIndex later)
{
    // when a node is displaced within a the tree, it's expected that the connections inside its descendance follow
    // thus it's needed to change the index of the displaced node when found as prefix of indices inside its descendance
    for (int i = 0; i < patterns.size(); i++)
    {
        for (int j = 0; j < patterns[i].components.size(); j++)
        {
            patterns[i].components[j].replacePrefix(former,later);
        }
    }
}

void Inheritance::mutate()
{
    // a lot of random alterations, weighted by several parameters
    Inheritance to_return;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine generator(seed);
    std::normal_distribution<float> distribution(0, mutability);

    std::normal_distribution<float> patterns_distribution(0, patterns_mutability);
    for (int i = 0; i < patterns.size(); i++)
    {
        for (int j = 0; j < patterns[i].components.size(); j++)
        {
            std::vector<int> insertions;
            std::vector<int> deletions;
            for (int k = 0; k < patterns[i].components[j].tree_int.size(); k++)
            {
                float depth_factor = 1 + sqrt(1.f / (1 + k));
                float x = patterns_distribution(generator);
                if (x > 2 * depth_factor)
                {
                    patterns[i].components[j].tree_int[k] = !patterns[i].components[j].tree_int[k];
                }

                float y = patterns_distribution(generator);
                if (y > 3 * depth_factor)
                {
                    x > 0 ? insertions.push_back(k) : deletions.push_back(k);
                }
            }
            while (insertions.size() != 0 || deletions.size() != 0)
            {
                if (insertions.size() > deletions.size())
                {
                    int k = deletions.back();
                    deletions.pop_back();
                    patterns[i].components[j].tree_int.erase(patterns[i].components[j].tree_int.begin() + k);
                }
                else
                {
                    int k = insertions.back();
                    insertions.pop_back();
                    patterns[i].components[j].tree_int.insert(patterns[i].components[j].tree_int.begin() + k, patterns_distribution(generator) > 0);
                }
            }
        }
        float sum = 0;
        for (int j = 0; j < patterns[i].probabilities.size(); j++)
        {
            patterns[i].probabilities[j] *= std::max(1 + patterns_distribution(generator), 0.5f);
            sum += patterns[i].probabilities[j];
        }

        for (int j = 0; j < patterns[i].probabilities.size(); j++)
        {
            patterns[i].probabilities[j] /= sum;
        }
    }
    to_return.patterns = patterns;

    std::normal_distribution<float> mutability_distribution(0, mutability_mutability);
    to_return.mutability = exp(log(mutability) + mutability_distribution(generator));

    std::normal_distribution<float> learning_strength_distribution(0, learning_strength_mutability);
    to_return.learning_strength = exp(log(learning_strength) + learning_strength_distribution(generator));

    std::normal_distribution<float> death_trigger_up_distribution(0, death_trigger_up_mutability);
    to_return.death_trigger_up = exp(log(death_trigger_up) + death_trigger_up_distribution(generator));

    std::normal_distribution<float> death_trigger_down_distribution(0, death_trigger_down_mutability);
    to_return.death_trigger_down = exp(log(death_trigger_down) + death_trigger_down_distribution(generator));

    std::normal_distribution<float> duplicate_trigger_distribution(0, duplicate_trigger_mutability);
    to_return.duplicate_trigger = exp(log(duplicate_trigger) + duplicate_trigger_distribution(generator));

    std::normal_distribution<float> temperature_change_high_distribution(0, temperature_change_high_mutability);
    to_return.temperature_change_high = exp(log(temperature_change_high) + temperature_change_high_distribution(generator));

    std::normal_distribution<float> temperature_change_low_distribution(0, temperature_change_low_mutability);
    to_return.temperature_change_low = exp(log(temperature_change_low) + temperature_change_low_distribution(generator));

    std::normal_distribution<float> temperature_up_distribution(0, temperature_up_mutability);
    to_return.temperature_up = exp(log(temperature_up) + temperature_up_distribution(generator));

    std::normal_distribution<float> temperature_divider_distribution(0, temperature_divider_mutability);
    to_return.temperature_divider = exp(log(temperature_divider - 1) + temperature_divider_distribution(generator)) + 1;

    std::normal_distribution<float> probabilities_resistance_distribution(0, probabilities_resistance_mutability);
    to_return.probabilities_resistance = exp(log(probabilities_resistance - 1) + probabilities_resistance_distribution(generator)) + 1;

    to_return.patterns_mutability = exp(log(patterns_mutability) + distribution(generator));
    to_return.learning_strength_mutability = exp(log(learning_strength_mutability) + distribution(generator));
    to_return.death_trigger_up_mutability = exp(log(death_trigger_up_mutability) + distribution(generator));
    to_return.death_trigger_down_mutability = exp(log(death_trigger_down_mutability) + distribution(generator));
    to_return.duplicate_trigger_mutability = exp(log(duplicate_trigger_mutability) + distribution(generator));
    to_return.temperature_change_high_mutability = exp(log(temperature_change_high_mutability) + distribution(generator));
    to_return.temperature_change_low_mutability = exp(log(temperature_change_low_mutability) + distribution(generator));
    to_return.temperature_up_mutability = exp(log(temperature_up_mutability) + distribution(generator));
    to_return.temperature_divider_mutability = exp(log(temperature_divider_mutability) + distribution(generator));
    to_return.probabilities_resistance_mutability = exp(log(probabilities_resistance_mutability) + distribution(generator));
    to_return.mutability_mutability = exp(log(mutability_mutability) + distribution(generator));
}