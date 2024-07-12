#include <bits/stdc++.h>
#include <omp.h>    
#include <stdlib.h>
#include <getopt.h>
#include "solver.h"
#include "graph.h"
#include "parameters.h"
#include "stat.h"

using namespace std;

static struct option long_options[] = {
    {"help",    no_argument,       0, 'h'},
    {"dataset", required_argument, 0, 'd'},
    {"salesmen", required_argument, 0, 's'},
    {"time",  optional_argument, 0, 'c'},
    {"iterations", optional_argument, 0, 'i'},
    {"threads", optional_argument, 0, 't'},
    {"variation", optional_argument, 0, 'v'},
    {"run", optional_argument, 0, 'r'},
    {0,         0,                 0,  0 }
};

int main(int argc, char **args){
    ios_base::sync_with_stdio(0); cin.tie(0);

    int option_index = 0;
    int c;

    string dataset;
    string variation;
    int salesmen = -1;
    int cutoff_iteration = 1e9;
    int cutoff_time = -1;
    int threads = 1;
    int run = 30;

    while (1) {
        c = getopt_long(argc, args, "hd:s:c:i:t:v:r:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 'h':
                printf("Usage: %s [options]\n", args[0]);
                printf("  -h, --help        Show this help message\n");
                printf("  -d, --dateset     Path to the dataset\n");
                printf("  -s, --salesmen    The number of salesmen\n");
                printf("  -t, --time        Cutoff time in second(s)\n");
                printf("  -i, --iterations  Cutoff iteration\n");
                printf("  -t, --threads     The number of CPU threads\n");
                printf("  -r, --run         The number of run\n"),
                printf("  -v, --variation   Variation\n");
                printf("\n");
                printf("Dataset and the number of salesmen are required.\n");
                printf("If neither the cutoff time nor the iteration is specified, the program will default to using a cutoff of 1000 iterations. If both are specified, then both will be used as stopping condition.\n");
                printf("If the number of run is not specified, the program will default to running 30 times.\n");
                exit(0);

            case 'd':
                dataset = string(optarg);
                break;

            case 's':
                salesmen = stoi(optarg);
                break;

            case 'c':
                cutoff_time = stoi(optarg);
                break;
            
            case 'i':
                cutoff_iteration = stoi(optarg);
                break;

            case 't':
                threads = stoi(optarg);
                break;

            case 'v':
                variation = string(optarg);
                break;

            case 'r':
                run = stoi(optarg);
                break;

            case '?':
                fprintf(stderr, "Unknown option: -%c\n", optopt);
                break;
            

            case ':':
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);

        }
    }

    if(cutoff_iteration == 1e9 && cutoff_time == -1){
        cutoff_iteration = 1000;
    }

    if(salesmen == -1 || dataset.empty()){
        cout << "Please specify the dataset and the number of salesmen." << endl;;
        return 0;
    }

    omp_set_num_threads(threads);

    system("mkdir convergence");
    system("mkdir result");

    Graph graph;
    graph.load_metadata(dataset);
    Stat stat(variation, graph, salesmen);

    for(int i = 0; i < run; i++){
        cout << endl << "Run " << i + 1 << ' ' << dataset << ' ' << salesmen << endl;
        
        auto start_time = chrono::high_resolution_clock::now();
        if(cutoff_time != -1 || i == 0) graph.load_data(dataset);
        mTSPSolver solver(graph, salesmen, cutoff_time, cutoff_iteration);
        solver.solve(stat, start_time);
        stat.write_result(solver.gbest, variation, graph, salesmen, i + 1);
    }
    stat.write_convergence(variation, graph, salesmen);
    stat.write_result_complete(variation, graph, salesmen);

    return 0;
}