#ifndef GRAPH_H
#define GRAPH_H

#include <bits/stdc++.h>
using namespace std;

struct Graph{
    int n;
    string EDGE_WEIGHT_TYPE, NAME;
    vector<vector<double>> distance;
    vector<vector<int>> closest, _closest;
    vector<double> x;
    vector<double> y;

    void init_closest();
    double calculate_distance(int i, int j);
    void load_metadata(const string &file);
    void load_data(const string &file);
    
};

#endif