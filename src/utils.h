#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
using namespace std;

extern mt19937 rng;
extern uniform_real_distribution<double> rng_real;

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