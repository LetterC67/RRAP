#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
using namespace std;

void init_random_number_generators(uint32_t seed);
mt19937& random_number_generator();
int rand_range(int l, int r);
double _qpow(double x, int y);

struct RouletteWheel{
    vector<double> probabilities;
    double sum = 0;
    double mx = 0;
    int pos = -1;

    void add(double probability);
    int spin();
};

#endif
