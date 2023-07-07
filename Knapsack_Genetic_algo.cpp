#include <bits/stdc++.h>
using namespace std;

const int n = 17;

// Arrays representing the values, weights, and volumes of items
array<int, n> value = {60, 100, 120, 80, 70, 60, 110, 75, 40, 90, 100, 150, 90, 145, 75, 100, 50};
array<int, n> weight = {10, 20, 30, 40, 50, 20, 35, 25, 10, 20, 60, 40, 20, 50, 50, 10, 50};
array<int, n> volume = {5, 3, 3, 2, 5, 2, 3, 2, 1, 2, 6, 4, 2, 5, 5, 3, 5};

int W = 150; // Maximum weight constraint
int V = 20;  // Maximum volume constraint

// Function to create an individual with random genes
vector<int> create_individual()
{
    vector<int> individual;
    for (int i = 0; i < n; ++i)
    {
        individual.push_back(rand() % 2);
    }
    return individual;
}

// Fitness function to evaluate the fitness of an individual
int fitness(const vector<int> &individual)
{
    int fitness = 0;
    int weight2 = 0;
    int volume2 = 0;
    for (int i = 0; i < n; ++i)
    {
        fitness += value[i] * individual[i];
        weight2 += weight[i] * individual[i];
        volume2 += volume[i] * individual[i];
    }
    if (weight2 > W || volume2 > V)
    {
        fitness = -1;
    }
    return fitness;
}

// Crossover operation to produce two children from two parents
pair<vector<int>, vector<int>> crossover(const vector<int> &parent1, const vector<int> &parent2)
{
    int position = 2 + rand() % (n - 3);
    vector<int> child1;
    vector<int> child2;
    for (int i = 0; i <= position; ++i)
    {
        child1.push_back(parent1[i]);
        child2.push_back(parent2[i]);
    }
    for (int i = position + 1; i < n; ++i)
    {
        child1.push_back(parent2[i]);
        child2.push_back(parent1[i]);
    }
    return make_pair(child1, child2);
}

// Mutation operation with probability 0.25
vector<int> mutation1(vector<int> individual)
{
    double probability = 0.25;
    double check = static_cast<double>(rand()) / RAND_MAX;
    if (check <= probability)
    {
        for (int i = 0; i < n; ++i)
        {
            double check1 = static_cast<double>(rand()) / RAND_MAX;
            if (check1 <= probability)
            {
                individual[i] = 1 - individual[i];
            }
        }
    }
    return individual;
}

// Mutation operation with probability 0.2
vector<int> mutation2(vector<int> individual)
{
    double probability = 0.2;
    double check = static_cast<double>(rand()) / RAND_MAX;
    if (check <= probability)
    {
        for (int i = 0; i < n; ++i)
        {
            double check1 = static_cast<double>(rand()) / RAND_MAX;
            if (check1 <= probability)
            {
                individual[i] = 1 - individual[i];
            }
        }
    }
    return individual;
}

// Tournament selection to select individuals for reproduction
vector<vector<int>> tournament_selection(const vector<vector<int>> &population)
{
    vector<vector<int>> new_population;
    for (int j = 0; j < 2; ++j)
    {
        vector<vector<int>> shuffled_population = population;
        random_shuffle(shuffled_population.begin(), shuffled_population.end());
        for (int i = 0; i < population.size() - 1; i += 2)
        {
            if (fitness(shuffled_population[i]) > fitness(shuffled_population[i + 1]))
            {
                new_population.push_back(shuffled_population[i]);
            }
            else
            {
                new_population.push_back(shuffled_population[i + 1]);
            }
        }
    }
    return new_population;
}

// Roulette wheel selection to select individuals for reproduction
vector<vector<int>> roulette_wheel_selection(const vector<vector<int>> &population)
{
    double total_fitness = 0;
    for (const vector<int> &individual : population)
    {
        total_fitness += 1.0 / (1 + fitness(individual));
    }

    vector<double> cumulative_fitness;
    double cumulative_fitness_sum = 0;
    for (int i = 0; i < population.size(); ++i)
    {
        cumulative_fitness_sum += 1.0 / (1 + fitness(population[i])) / total_fitness;
        cumulative_fitness.push_back(cumulative_fitness_sum);
    }

    vector<vector<int>> new_population;
    for (int i = 0; i < population.size(); ++i)
    {
        double roulette = static_cast<double>(rand()) / RAND_MAX;
        for (int j = 0; j < population.size(); ++j)
        {
            if (roulette <= cumulative_fitness[j])
            {
                new_population.push_back(population[j]);
                break;
            }
        }
    }

    return new_population;
}

int main()
{
    srand(time(NULL));

    int population_size = 200;
    int generation = 0;
    vector<vector<int>> population;
    for (int i = 0; i < population_size; ++i)
    {
        vector<int> individual = create_individual();
        population.push_back(individual);
    }

    int best_fitness = fitness(population[0]);
    vector<int> fittest_individual = population[0];
    int gen = 0;
    int answer = best_fitness;
    vector<int> all_best;

    while (gen != 100)
    {
        gen++;
        population = tournament_selection(population);
        vector<vector<int>> new_population;
        for (int i = 0; i < population_size - 1; i += 2)
        {
            pair<vector<int>, vector<int>> children = crossover(population[i], population[i + 1]);
            new_population.push_back(children.first);
            new_population.push_back(children.second);
        }
        int tot_fitness = 0;
        for (const vector<int> &individual : new_population)
        {
            tot_fitness += fitness(individual);
        }
        double avg_fitness = static_cast<double>(tot_fitness) / population_size;
        for (vector<int> &individual : new_population)
        {
            if (fitness(individual) < avg_fitness)
            {
                individual = mutation1(individual);
            }
            else
            {
                individual = mutation2(individual);
            }
        }
        population = new_population;
        best_fitness = fitness(population[0]);
        for (const vector<int> &individual : population)
        {
            if (fitness(individual) > best_fitness)
            {
                best_fitness = fitness(individual);
                fittest_individual = individual;
            }
            all_best.push_back(best_fitness);
            answer = max(answer, best_fitness);
        }
        if (gen % 1 == 0)
        {
            cout << "Generation: " << gen << " Best Fitness: " << best_fitness << " Individual: ";
            for (int gene : fittest_individual)
            {
                cout << gene << " ";
            }
            cout << endl;
        }
    }

    // Print the best fitness and the corresponding individual
    cout << "Best Fitness: " << answer << " Individual: ";
    for (int gene : fittest_individual)
    {
        cout << gene << " ";
    }
    cout << endl;

    return 0;
}
