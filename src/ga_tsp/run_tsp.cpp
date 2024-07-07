#ifndef __ENVIRONMENT__
#include "environment.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

pair<double, vector<int>> run(int N, vector<vector<double>> &distances){
	InitURandom(); 
	int maxNumOfTrial;

	TEnvironment* gEnv = new TEnvironment();
	gEnv->fFileNameTSP=(char*)malloc(100);
	
	maxNumOfTrial = 1; // repeated times
	gEnv->Npop = 100; // number of items
	gEnv->Nch = 30; // number of offsprings

	//cout << "Initializing ..." << endl;
	gEnv->define(N, distances);
	///cout << "Building solution ..." << endl;
	for (int n = 0; n < maxNumOfTrial; ++n){ 
		gEnv->doIt(); 
		gEnv->printOn(n);
	}
	auto result = gEnv -> bestSolution();
	delete gEnv;
	gEnv = nullptr;
    return result;
}