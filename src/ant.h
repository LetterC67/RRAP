#ifndef ANT_H
#define ANT_H

#include <bits/stdc++.h>
#include "parameters.h"
#include "graph.h"
using namespace std;

struct _tour{
    vector<int> tour;
    double cost;

    _tour(){
        cost = 0.;
        tour = {DEPOT};
    }

    int& operator[](int index) {
        return tour[index];
    }

    const int& operator[](int index) const {
        return tour[index];
    }
    
    auto begin() {
        return tour.begin();
    }

    auto end() {
        return tour.end();
    }

    void push_back(int u){
        tour.push_back(u);
    }

    int back(){
        return tour.back();
    }

    int size(){
        return tour.size();
    }

    friend bool operator<(const _tour& lhs, const _tour& rhs) {
        return lhs.cost < rhs.cost;
    }

    friend bool operator>(const _tour& lhs, const _tour& rhs) {
        return lhs.cost > rhs.cost;
    }
};

struct Ant{      
    vector<vector<double>> *distance;
    Graph *graph;
    vector<_tour> tours;
    vector<int> assigned, position;
    unordered_set<int> del;
    double min_max_cost;
    double min_sum_cost;

    Ant();

    Ant(int salesmen, vector<vector<double>> *distance, Graph *graph);

    friend bool operator<(const Ant& lhs, const Ant& rhs);

    void retag(int index);
    double tour_length(_tour &tour);

    void add(int salesman, int vertex);
    void add(_tour &tour, int vertex);
    
    bool relocate(_tour &a, _tour &b, int idx_a, int idx_b);

    bool two_opt_sweepline(_tour &tour, int idx);
    bool two_opt_inter_tour(_tour &a, _tour &b, int idx_a, int idx_b);
    bool or_opt(_tour &tour, int idx);
    void run_tsp();
    void verify(int n);

    bool intra_tour_optimization();

    void local_search();
    void end_tour();
    void calculate_result();
};

Ant trim(Ant ant);

#endif