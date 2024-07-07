#include <bits/stdc++.h>
#include <chrono>
#include "graph.h"
#include "solver.h"
#include "ant.h"
#include "parameters.h"
#include "utils.h"
#include "stat.h"

using namespace std;


mTSPSolver::mTSPSolver(Graph &graph, int salesmen, int cutoff_time, int cutoff_iteration): graph(graph), salesmen(salesmen), cutoff_iteration(cutoff_iteration), cutoff_time(cutoff_time){
    n = graph.n;
    TAU_MIN = 1 / (double(n * n) / RATIO);
    TAU_MAX = 1.;
    pheromone.resize(n);
    for(auto &p : pheromone)
        p = vector<double>(n, TAU_MAX);
    population = Population(n, salesmen);
}

pair<int, int> mTSPSolver::select_city(Ant &ant, vector<bool> &visited){
    RouletteWheel wheel;
    int salesman = min_element(ant.tours.begin(), ant.tours.end()) - ant.tours.begin();
    int current_city = ant.tours[salesman].back();

    vector<int> candidate;
    int bound = min(N_NEAREST, (int)ant.graph -> _closest[current_city].size());
    for(int i = 0; i < bound; i++){
        int city = ant.graph -> _closest[current_city][i];
        if(!visited[city]){
            double prob = pheromone[current_city][city] * 1. / _qpow(graph.distance[current_city][city], BETA);
            wheel.add(prob);
            candidate.push_back(city);
        }
        if(i == bound - 1 && !candidate.size()){
            bound = ant.graph -> _closest[current_city].size();
        }
    }

    assert(candidate.size());

    return {salesman, candidate[wheel.spin()]};
}

Ant mTSPSolver::build_solution(Ant ant){
    vector<bool> visited(n);

    for(auto &tour : ant.tours){
        for(auto &vertex : tour){
            visited[vertex] = true;
        }
    }
    visited[DEPOT] = true;

    int not_visited = count(visited.begin(), visited.end(), 0);

    while(not_visited--){
        auto next = select_city(ant, visited);
        ant.add(next.first, next.second);
        visited[next.second] = true;
    }

    ant.end_tour();
    ant.local_search();
    ant.calculate_result();

    return ant;
}

vector<Ant> mTSPSolver::build_solutions(){
    vector<Ant> ants;

    #pragma omp parallel for
    for(int ant = 0; ant < (iteration == 1 ? MU : LAMBDA); ant++){
        Ant a;
        if(!population.population.size())
            a = build_solution(Ant(salesmen, &graph.distance, &graph));
        else{
            a = build_solution(trim(population.get()));
        }
        #pragma omp critical
        {
            ants.push_back(a);
        }
    }

    return ants;
}

void mTSPSolver::update_pheromone(){
    auto p = pheromone;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            pheromone[i][j] = pheromone[i][j] * (1 - RHO) + TAU_MIN * RHO;
        }
    }

    for(auto &tour : gbest.tours){
        for(int i = 0; i < tour.size() - 1; i++){
            pheromone[tour[i]][tour[i + 1]] = p[tour[i]][tour[i + 1]] * (1 - RHO) + TAU_MAX * RHO;
        }
    }
}

void mTSPSolver::solve(Stat &stat){
    auto start_time = chrono::high_resolution_clock::now();

    
    for(; iteration <= cutoff_iteration; iteration++){
        auto start_iteration = chrono::high_resolution_clock::now();

        Ant ibest;
        auto ants = build_solutions();

        for(auto &ant : ants){
            population.add(ant);
            if(ant < gbest){
                if(iteration > RUN_TSP_THRESHOLD) ant.run_tsp();
                gbest = ant;
                no_improve = 0;
            }

            if(ant < ibest){
                ibest = ant;
            }
        }

        if(iteration == RUN_TSP_THRESHOLD){
            gbest.run_tsp();
            population.add(gbest);
        }
        update_pheromone();
        population.kill();

        stat.add(iteration - 1, gbest.min_max_cost);

        auto end_itertation = chrono::high_resolution_clock::now();
        double iteration_time = chrono::duration_cast<chrono::nanoseconds>(end_itertation - start_iteration).count() * 1e-9;
        double program_time = chrono::duration_cast<chrono::nanoseconds>(end_itertation - start_time).count() * 1e-9;

         cout << "Iteration " << iteration << ' ' << gbest.min_max_cost << ' ' << ibest.min_max_cost << ' ' << no_improve << ' ' << iteration_time << endl;

        if(cutoff_time != -1 && program_time > cutoff_time) break;

        for(int i = 0; i < n; i++){
            sort(graph.closest[i].begin(), graph.closest[i].end(), [&](const int &a, const int &b){
                if(gbest.assigned[a] != gbest.assigned[b])
                    return gbest.assigned[a] < gbest.assigned[b];
                return gbest.position[a] < gbest.position[b];
            });
        }
    }
    gbest.verify(n);

    cout << "Result: " << gbest.min_max_cost << endl;
}
