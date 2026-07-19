#include <bits/stdc++.h>
#include <omp.h>
#include <stdlib.h>
#include <getopt.h>
#include "solver.h"
#include "graph.h"
#include "parameters.h"
#include "stat.h"
#include "utils.h"

using namespace std;

#include "include/json/json.hpp"
using json = nlohmann::ordered_json;

int main(int argc, char* argv[]){
    ios_base::sync_with_stdio(0); cin.tie(0);

	PARAMETER = parse_parameter_options(argc, argv);
	if (PARAMETER.SEED == 0) {
		std::random_device rd;
		PARAMETER.SEED = rd();
	}
    init_random_number_generators(PARAMETER.SEED);

    // nlohmann::ordered_json experiment_records;
    // transform(PARAMETER, experiment_records);
    // std::ofstream json_out("parameters.json", std::ios::out);
	// json_out << experiment_records.dump(2);
	// json_out.close();

    if(PARAMETER.SALESMEN == -1){
        cout << "Please specify the number of salesmen." << endl;;
        return 0;
    }

    omp_set_num_threads(PARAMETER.NUM_THREADS);

    system("mkdir convergence");
    system("mkdir result");

    Graph graph;
    graph.load_metadata(PARAMETER.INSTANCE_PATH);
    Stat stat(PARAMETER.ALGORITHM_NAME, graph);

    if(PARAMETER.CUTOFF_ITERATIONS == INT32_MAX && PARAMETER.CUTOFF_TIME == -1){
        PARAMETER.CUTOFF_TIME = 240.0 * graph.n / 100;
        // PARAMETER.CUTOFF_TIME = 240.0 * graph.n / (100 * 4);
        // PARAMETER.CUTOFF_TIME = 240.0 * graph.n / (100 * PARAMETER.NUM_THREADS); // for fast testing
    }

    for(int i = 0; i < PARAMETER.NUM_RUNS; i++){
        cout << endl << "Run " << i + 1 << ' ' << PARAMETER.INSTANCE_PATH << ' ' << PARAMETER.SALESMEN << endl;
        
        auto start_time = chrono::high_resolution_clock::now();
        if(PARAMETER.CUTOFF_TIME != -1 || i == 0) graph.load_data(PARAMETER.INSTANCE_PATH);
        mTSPSolver solver(graph, PARAMETER.SALESMEN, PARAMETER.CUTOFF_TIME, PARAMETER.CUTOFF_ITERATIONS);
        solver.solve(stat, start_time);
        stat.write_result(solver.gbest, PARAMETER.ALGORITHM_NAME, graph, i + 1);
    }
    stat.write_convergence(PARAMETER.ALGORITHM_NAME, graph);
    stat.write_result_complete(PARAMETER.ALGORITHM_NAME, graph);

    return 0;
}