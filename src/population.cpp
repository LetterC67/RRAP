#include <bits/stdc++.h>
#include "ant.h"
#include "utils.h"
#include "parameters.h"
#include "population.h"

Population::Population(){}
Population::Population(int n, int salesmen): n(n), salesmen(salesmen) {};

double Population::ant_distance(Ant &a, unordered_set<int> &edges){
    double cnt = 0;
    for(auto &v : a.tours){
        for(int i = 0; i < v.size() - 1; i++){
            if(edges.count(v[i] * n + v[i + 1])) cnt += 1.;
        }
    }

    return cnt / double(n + salesmen - 1);
}

void Population::kill(){
    vector<double> fitness(population.size());
    vector<int> index;

    for(int i = 0; i < population.size(); i++){
        double mx = 0.;
        index.push_back(i);
        vector<double> v;
        unordered_set<int> edges;
        
        for(auto &v : population[i].tours){
            for(int i = 0; i < v.size() - 1; i++){
                edges.insert(v[i] * n + v[i + 1]);
            }
        }

        for(int j = population.size() - 1; j >= 0; j--){
            v.push_back(ant_distance(population[j],edges));
        }

        sort(v.begin(), v.end(), greater<double>());

        fitness[i] = population[i].min_max_cost * powl(DELTA, (v[0] + v[1]) / 2.);
    }

    sort(index.begin(), index.end(), [&](const int &a, const int &b){
        return fitness[a] < fitness[b];
    });

    vector<Ant> temp;

    for(int i = 0; i < min((int)population.size(), MU); i++){
        temp.push_back(population[index[i]]);
    }

    population = temp;
}

void Population::add(Ant &ant){
    population.push_back(ant);
}

Ant Population::get(){
    return population[rng() % population.size()];
}