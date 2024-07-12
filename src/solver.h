#ifndef SOLVER_H
#define SOLVER_H

#include <bits/stdc++.h>
#include "graph.h"
#include "parameters.h"
#include "ant.h"
#include "stat.h"
#include "population.h"

using namespace std;


struct mTSPSolver{
    Graph graph;
    int n;
    int salesmen;
    int iteration = 1;
    int no_improve = 0;
    double TAU_MIN, TAU_MAX;

    int cutoff_time, cutoff_iteration;

    Ant gbest;
    Population population;
    vector<vector<double>> pheromone;


    mTSPSolver(Graph &graph, int salesmen, int cutoff_time, int cutoff_iteration);

    void solve(Stat &stat, chrono::time_point<chrono::high_resolution_clock> start_time);
    void update_pheromone();
    vector<Ant> build_solutions();
    Ant build_solution(Ant ant);
    pair<int, int> select_city(Ant &ant, vector<bool> &visited);
};

#endif