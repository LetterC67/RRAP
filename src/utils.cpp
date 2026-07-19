#include <bits/stdc++.h>
#include <atomic>
#include <omp.h>
#include "utils.h"
#include "ant.h"
#include "parameters.h"

using namespace std;

namespace {
uint32_t base_seed = 0;
atomic<uint64_t> seed_generation{0};
thread_local uint64_t local_seed_generation = 0;
thread_local mt19937 thread_rng;
}

void init_random_number_generators(uint32_t seed) {
    base_seed = seed;
    seed_generation.fetch_add(1, memory_order_release);
    random_number_generator();
}

mt19937& random_number_generator() {
    const uint64_t generation = seed_generation.load(memory_order_acquire);
    if (local_seed_generation != generation) {
        const uint32_t thread_id = omp_in_parallel()
            ? static_cast<uint32_t>(omp_get_thread_num())
            : 0U;
        seed_seq seeds{base_seed, thread_id, 0x52524150U};
        thread_rng.seed(seeds);
        local_seed_generation = generation;
    }
    return thread_rng;
}

double _qpow(double x, int y){
    double res = 1;
    while(y--) res *= x;
    return res;
}

int rand_range(int l, int r){
    return l + random_number_generator()() % (r - l + 1);
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
    thread_local uniform_real_distribution<double> rng_real(0.0, 1.0);
    double threshold = rng_real(random_number_generator());
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
