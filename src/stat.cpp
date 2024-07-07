#include <bits/stdc++.h>
#include "stat.h"
#include "graph.h"
using namespace std;

Stat::Stat(string &variation, Graph &graph, int salesmen){
    std::ofstream outputFile("result/" + graph.NAME + "_" + to_string(salesmen) + "_" + variation, std::ios::out);
    best = 1e18;
    cnt = worst = avg = 0;
}


void Stat::write_result(Ant &gbest, string &variation, Graph &graph, int salesmen, int run){
    cnt++;
    worst = max(worst, gbest.min_max_cost);
    best = min(best, gbest.min_max_cost);
    avg += gbest.min_max_cost;

    std::ofstream outputFile("result/" + graph.NAME + "_" + to_string(salesmen) + "_" + variation, std::ios::app);
    
    outputFile <<"Run " << run << ' ' << gbest.min_max_cost << endl;
    for(auto &tour : gbest.tours){
        for(int d : tour) outputFile << d << ' ';
        outputFile << endl;
    }
    for(auto &tour : gbest.tours){
        outputFile << tour.cost << ' ';
    }
    outputFile << endl;
    outputFile << endl;

    outputFile.close();
}

void Stat::add(int iter, double cost){
    if(iter >= convergence.size()){
        convergence.push_back(0.);
        ccnt.push_back(0);
    }
    convergence[iter] += cost;
    ccnt[iter]++;
}

void Stat::write_convergence(string &variation, Graph &graph, int salesmen){
    std::ofstream outputFile("convergence/" + graph.NAME + "_" + to_string(salesmen) + "_" + variation, std::ios::out);

    for(int i = 0; i < convergence.size(); i++){
        outputFile << i + 1 << ' ' << convergence[i] / double(ccnt[i]) << endl;
    }

    outputFile.close();
}

void Stat::write_result_complete(string &variation, Graph &graph, int salesmen){
    std::ofstream outputFile("result/" + graph.NAME + "_" + to_string(salesmen) + "_" + variation, std::ios::app);
    std::ofstream tuningFile("tuning/" + graph.NAME + "_" + to_string(salesmen), std::ios::app);
    
    outputFile <<"Avg: " << avg / double(cnt) << endl;
    outputFile <<"Best: " << best << endl;
    outputFile <<"worst: " << worst << endl;
}