#include <bits/stdc++.h>
#include "utils.h"
#include "ant.h"
#include "parameters.h"

using namespace std;

mt19937 rng;
uniform_real_distribution<double> rng_real(0.0, 1.0);

void init_random_number_generators(uint32_t seed) {
    rng = std::mt19937(seed);
}

double _qpow(double x, int y){
    double res = 1;
    while(y--) res *= x;
    return res;
}

int rand_range(int l, int r){
    return l + rng() % (r - l + 1);
}

void RouletteWheel::add(double probability){
    probabilities.push_back(probability);
    sum += probability;
    if(mx < probability){
        mx = probability;
        pos = probabilities.size() - 1;
    }
}

int RouletteWheel::spin(){
    double threshold = rng_real(rng);
    double accumulation = 0.;

    for(int i = 0; i < probabilities.size(); i++){
        accumulation += probabilities[i];
        if(accumulation >= threshold * sum){
            return i;
        }
    }

    return probabilities.size() - 1;
}

double Ant::tour_length(_tour &tour){
    double cost = 0;
    for(int i = 0; i < tour.size() - 1; i++){
        cost += (*distance)[tour[i]][tour[i + 1]];
    }
    return cost;
}