/*
 * evaluator.h
 *   created on: April 24, 2013
 * last updated: May 10, 2020
 *       author: Shujia Liu
 */

#ifndef __EVALUATOR__
#define __EVALUATOR__

#ifndef __INDI__
#include "indi.h"
#endif

#include <string.h>
#include <assert.h>
#include <vector>
#include <string>
using namespace std;

class TEvaluator{
public:
	TEvaluator();
	~TEvaluator();
	void setInstance(const string& filename); // sets variables
	void setInstance(int N, vector<vector<double>> &distances); // sets variables
	void doIt( TIndi& indi ); // sets indi.fEvaluationValue
	void writeTo( FILE* fp, TIndi& indi ); // prints out TSP solution
	pair<double, vector<int>> returnBestSolution(TIndi& indi); // returns the best TSP solution
	bool checkValid(vector<int>& array, double value ); // checks if TSP solution is valid

	int fNearNumMax; // the maximum value of the number of nearby points
	vector<vector<int>> fNearCity; // NearCity[i][k] is the k points that with a shortest distance from point i
	vector<vector<double>> fEdgeDis; // EdgeDis[i][j] is the distance from city i to city j
	int Ncity; // the number of cities
	vector<double> x; // x[i] is the x coordinate of city i
	vector<double> y; // y[i] is the y coordinate of city i
	vector<int> Array; // the index of best solution
};

#endif
