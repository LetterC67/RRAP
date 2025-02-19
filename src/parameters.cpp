#include "parameters.h"

const int BETA = 5;
const double RHO_BASE = .02;
const double RHO_COEFF = 1. / 1500.;
const int DEPOT = 0;
const int NUM_THREADS = 1;
const double RATIO = 80;
const int OPT_SIZE = 4;
const double DELTA = 1.2;
const int MU = 15;     // The number of ants in the initial iteration
const int LAMBDA = 15; // the number of ants in subsequent iterations
const int N_NEAREST = 100;
const int ALPHA = 15;
const int RUN = 30;
const int RUN_TSP_THRESHOLD = 20; // Only trigger EAX-TSP after a certain number of iterations