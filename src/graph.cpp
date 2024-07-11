#include <bits/stdc++.h>
#include "graph.h"
#include "parameters.h"
using namespace std;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ (hash2 << 1); // Combine the two hash values
    }
};

struct OneMinimumSpanningTree {
    int sourceNode;
    int numNodes;
    vector<int> par;
    vector<double> d;
    vector<int> topo;
    vector<bool> visited;
    unordered_set<pair<int,int>, pair_hash> edges;
    vector<vector<int>> g;
    int chooseRate;

    vector<int> bestPar;
    vector<int> bestTopo;
    vector<double> bestW;
    unordered_set<pair<int, int>, pair_hash> bestEdges;
    vector<vector<double>> dis;

    /*
     * Implement Prim Algorithm to find the minimum spanning tree without the source node
     * Find the two nodes that have the minimum distance to source node -> A 1-Tree Minimum Spanning Tree is found.
     * For all edge (i, j), find beta(i, j) and compute alpha(i, j) = beta(i, j) - dis(i, j)
     * The function will return for each node the 0.1 * numNodes closest to it base on alpha
    */

    OneMinimumSpanningTree(int _sourceNode, int _numNodes, int _chooseRate,  vector<vector<double>> dis): 
        sourceNode(_sourceNode), 
        numNodes(_numNodes),
        d(_numNodes),
        par(_numNodes),
        visited(_numNodes),
        chooseRate(_chooseRate),
        dis(dis),
        g(_numNodes) {

        }

    void reset(){
        for (int i = 0; i < numNodes; i++) {
            d[i] = 1e9;
            visited[i] = false;
            par[i] = -1;
            g[i].clear();
        }
        topo.clear();
        edges.clear();
    }

    double primAlgorithm(vector<double> &w) {
        // Remember to exclude the source node
        reset();
        double totalWeight = 0;

        d[sourceNode + 1] = 0;
        
        for (int iter = 1; iter <= numNodes - 1; iter++) {
            int bestNode;
            double Min = 1e9 + 1;

            for (int node = 1; node < numNodes; node++){
                if (!visited[node] && d[node] < Min){
                    Min = d[node];
                    bestNode = node;
                }
            }

            totalWeight += d[bestNode];
            topo.push_back(bestNode);
            if (par[bestNode] != -1){
                edges.insert({bestNode, par[bestNode]});
                g[par[bestNode]].push_back(bestNode);
                g[bestNode].push_back(par[bestNode]);
            }
            visited[bestNode] = true;

            for (int node = 1; node < numNodes; node++){
                if (!visited[node] && d[node] > dis[bestNode][node] + w[bestNode] + w[node]) {
                    d[node] = dis[bestNode][node] + w[bestNode] + w[node];
                    par[node] = bestNode;
                }
            }
        }
        checkValid();
        return totalWeight;
    }
    

    void dfs(int node, int p, vector<bool> &vis){
        vis[node] = true;
        for (auto &v : g[node]){
            if (p == v) continue;
            if (!vis[v]){
                if (par[v] != node){

                }
                dfs(v, node, vis);
            } else {
                assert(false);
            }
        }
    }

    void checkValid(){
        vector<bool> vis(numNodes);
        dfs(1, 0, vis);
        for (int i = 1; i < numNodes; i++){
            if (!vis[i]){
                assert(false);
            }
        }
    }

    void a_improved(){
        vector<double> w(numNodes, 0);
        double W = -1e18;
        double step_size = 1;
        int period = numNodes / 4;
        int iter = period;
        bool isFirstPeriodIncreasing = true;
        vector<int> prev_v(numNodes, 0);
        

        const double eps = 1e-6;
        while (period > 0 && step_size > eps){
            iter--;
            double newWeight = primAlgorithm(w);
            newWeight -= 2 * accumulate(w.begin(), w.end(), 0.0);
            bool increase = false;

            if (newWeight > W){
                bestPar = par;
                bestTopo = topo;
                bestW = w;
                W = newWeight;
                bestEdges = edges;
                increase = true;
            }
            isFirstPeriodIncreasing &= increase;


            // compute for the next
            vector<int> v(numNodes);

            bool checkFullZero = true;
            for (int i = 0; i < numNodes; i++){
                v[i] = (int)g[i].size() - 2;
                if (v[i] != 0) checkFullZero = false;
            }

            if (checkFullZero) break;

            if (isFirstPeriodIncreasing){
                step_size *= 2;
            }

            for (int i = 0; i < numNodes; i++){
                w[i] += step_size * v[i];
            }

            if (iter == 0){
                if (increase){
                    iter = period;
                } else{
                    period /= 4;
                    step_size /= 2;
                    iter = period;
                }
            }
        }
    }

    double new_dis(int i, int j){
        if (i == j) return 0;
        return dis[i][j] + bestW[i] + bestW[j];
    }
    vector<vector<int>> computeCandidateSet() {
        a_improved();
        vector<vector<int>> candidateSet;

        // find the best two
        vector<pair<double, int>> c;
        for (int node = 1; node < numNodes; node++) {
            c.push_back({new_dis(sourceNode, node), node});
        }
        sort(c.begin(), c.end());

        bestEdges.insert({sourceNode, c[0].second});
        bestEdges.insert({sourceNode, c[1].second});

        vector<pair<double, int>> candidates;

        for (int i = 1; i < numNodes; i++) {
            candidates.push_back({new_dis(i, sourceNode) - new_dis(sourceNode, c[1].second), i});
        }

        sort(candidates.begin(), candidates.end());
        vector<int> candidate;

        for (int j = 0; j < chooseRate; j++){
            candidate.push_back(candidates[j].second);
        }
        candidateSet.push_back(candidate);

        candidates.clear();
        candidate.clear();

        vector<int> marked(numNodes);
        vector<double> b(numNodes);
        for (int i = 1; i < numNodes; i++){
            marked[i] = 0;
        }

        for (int node = 1; node < numNodes; node++) {
            ///// BUILD THE 1-TREE //////////////////////////
            b[node] = -INT_MAX;
            for (int k = node; k != 1; k = bestPar[k]) {
                b[bestPar[k]] = max(b[k], new_dis(k, bestPar[k]));
                marked[k] = node;
            }

            for (auto &v : bestTopo) {
                if (v != node) {
                    if (marked[v] != node) {
                        b[v] = max(b[bestPar[v]], new_dis(v, bestPar[v]));
                    }
                }
            }
            /////////////////////////////////////////////////
            for (int v = 1; v < numNodes; v++) {
                if (v == node) continue;
                candidates.push_back({new_dis(node, v) - b[v], v});
            }
            sort(candidates.begin(), candidates.end());

            for (int j = 0; j < chooseRate; j++){
                candidate.push_back(candidates[j].second);
            }
        
            candidateSet.push_back(candidate);
            candidate.clear();
            candidates.clear();
        }
        return candidateSet;
    }
};


double Graph::calculate_distance(int i, int j) {
    if(EDGE_WEIGHT_TYPE == "ATT"){
        return (int)ceil(sqrt(((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])) / 10.));
    }else{
        return sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
    }
}

void Graph::init_closest(){
    OneMinimumSpanningTree one_tree(0, n, n - 1, distance);
    _closest = one_tree.computeCandidateSet();
    closest = _closest;

    for(auto &c : closest){
        while(c.size() > ALPHA) c.pop_back();
    }
}

void Graph::load_data(const string &file){
    cout << "Load data from " << file << "..." << endl;
    ifstream ifs(file, ifstream::in);
    string nil;

    ifs >> nil; 
    if(nil.back() != ':') ifs >> nil;


    ifs >> NAME;
    ifs.ignore();

    getline(ifs, nil);
    getline(ifs, nil);
    
    ifs >> nil; if(nil.back() != ':') ifs >> nil;
    ifs >> n;
    ifs >> nil; if(nil.back() != ':') ifs >> nil;
    ifs >> EDGE_WEIGHT_TYPE;

    ifs >> nil;

    closest = vector<vector<int>>(n);

    x.resize(n);
    y.resize(n);
    
    for(int i = 0, temp; i < n; i++){
        ifs >> temp;
        ifs >> x[i] >> y[i];
    }

    distance.resize(n);
    for(auto &d : distance) d.resize(n);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            distance[i][j] = calculate_distance(i, j);
        }
    }

    init_closest();

    cout << "Loaded data from " << file << endl;
}