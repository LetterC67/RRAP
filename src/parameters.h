#ifndef PARAMETERS_H
#define PARAMETERS_H


#include <stdint.h>
#include <string>

struct Parameter {
	struct SMMASParameter {
		// The relative influence of the pheromone intensity and the heuristic information
		int BETA = 5;
		
		// The evaporation rate of the pheromone
		double RHO_BASE = .02;
		double RHO_COEFF = 1. / 1500.;

		// The number of ants
		int LAMBDA = 15;

		double RATIO = 80; // Use for tau min
	};

	SMMASParameter SMMAS_PARAMETER;

	// The size of the population
	int MU = 15;

	double DELTA = 1.2;

	double MAXIMUM_RUIN_AND_RECREATE_RATE = 0.5;

	int N_NEAREST = 100;

	// a-nearness in 1-tree
	int ALPHA = 15;

	// // Only trigger EAX-TSP after a certain number of iterations
	int RUN_TSP_THRESHOLD = 20;

	int SALESMEN = -1;

	int CUTOFF_ITERATIONS = INT32_MAX;
	double CUTOFF_TIME = -1;

	// The default path to store results
	std::string OUTPUT_PATH = "instances/output";

	std::string INSTANCE_PATH = "dataset/pcb3038.tsp";

	std::string ALGORITHM_NAME = "RRAP";

	int NUM_RUNS = 1;

	int NUM_THREADS = 1;

	/**
	 * Random number generator seed
	 * 0 means that seed should be based on the built-in std::random_device
	 */
	uint32_t SEED = 0;
};

extern Parameter PARAMETER;

template<typename MapT>
void transform(const Parameter& parameter, MapT& map) {
	map["ants"] = parameter.SMMAS_PARAMETER.LAMBDA;
	map["beta"] = parameter.SMMAS_PARAMETER.BETA;
	map["rho base"] = parameter.SMMAS_PARAMETER.RHO_BASE;
	map["rho coef"] = parameter.SMMAS_PARAMETER.RHO_COEFF;
	map["ratio"] = parameter.SMMAS_PARAMETER.RATIO;
	map["population"] = parameter.MU;
	map["delta"] = parameter.DELTA;
	map["maximum Ruin and Recreate rate"] = parameter.MAXIMUM_RUIN_AND_RECREATE_RATE;
	map["n-nearness"] = parameter.N_NEAREST;
	map["a-nearness"] = parameter.ALPHA;
	map["EAX-TSP trigger"] = parameter.RUN_TSP_THRESHOLD;
	map["salesmen"] = parameter.SALESMEN;
	map["iterations"] = parameter.CUTOFF_ITERATIONS;
	map["time"] = parameter.CUTOFF_TIME;
	map["output path"] = parameter.OUTPUT_PATH;
	map["instance path"] = parameter.INSTANCE_PATH;
	map["alg name"] = parameter.ALGORITHM_NAME;
	map["repeat"] = parameter.NUM_RUNS;
	map["threads"] = parameter.NUM_THREADS;
	map["seed"] = parameter.SEED;
}

Parameter parse_parameter_options(int argc, char* argv[]);

extern const int DEPOT;
extern const int OPT_SIZE;

#endif