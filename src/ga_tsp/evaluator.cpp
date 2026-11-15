/*
 * evaluator.cpp
 *   created on: April 24, 2013
 * last updated: May 10, 2020
 *       author: Shujia Liu
 */

#ifndef __EVALUATOR__
#include "evaluator.h"
#endif
#include <math.h>
#include <iostream>
using namespace std;

TEvaluator::TEvaluator() {
	Ncity = 0;
	fNearNumMax = 50;
}

TEvaluator::~TEvaluator() {}

void TEvaluator::setInstance(int N, vector<vector<double>> &distances){
	Ncity = N;
	fEdgeDis = distances;

	fNearCity.clear();
	for (int i = 0; i < Ncity; i++) {
		vector<int> row(fNearNumMax + 1);
		fNearCity.push_back(row);
	}

	vector<int> checkedN(Ncity);

	int ci, j1, j2, j3;
	int cityNum = 0;
	double minDis;
	for( ci = 0; ci < Ncity; ++ci ){
		for( j3 = 0; j3 < Ncity; ++j3 ) checkedN[ j3 ] = 0;
		checkedN[ ci ] = 1;
		fNearCity[ ci ][ 0 ] = ci;
		for( j1 = 1; j1 <= fNearNumMax; ++j1 ) {
			minDis = 100000000;
			for( j2 = 0; j2 < Ncity; ++j2 ){
				if( fEdgeDis[ ci ][ j2 ] <= minDis && checkedN[ j2 ] == 0 ){
					cityNum = j2;
					minDis = fEdgeDis[ ci ][ j2 ];
				}
			}
			fNearCity[ ci ][ j1 ] = cityNum;
			checkedN[ cityNum ] = 1;
		}
	}
}

void TEvaluator::setInstance(const string& filename) {
	FILE* fp;
	int n;
	char word[ 80 ], type[ 80 ];
	fp = fopen(filename.c_str(), "r");
	while( 1 ){
		if( fscanf( fp, "%s", word ) == EOF ) break;
		if( strcmp( word, "DIMENSION" ) == 0 ){
			fscanf( fp, "%s", word );
			fscanf( fp, "%d", &Ncity );
		}
		if( strcmp( word, "EDGE_WEIGHT_TYPE" ) == 0 ){
			fscanf( fp, "%s", word );
			fscanf( fp, "%s", type );
		}
		if( strcmp( word, "NODE_COORD_SECTION" ) == 0 ) break;
	}
	if( strcmp( word, "NODE_COORD_SECTION" ) != 0 ){
		printf( "Error in reading the instance\n" );
		exit(0);
	}
	x.resize(Ncity);
	y.resize(Ncity);
	vector<int> checkedN(Ncity);

	for( int i = 0; i < Ncity; ++i ){
		fscanf( fp, "%d", &n );
		fscanf( fp, "%s", word );
		x[ i ] = atof( word );
		fscanf( fp, "%s", word );
		y[ i ] = atof( word );
	}
	fclose(fp);

	fEdgeDis.clear();
	for (int i = 0; i < Ncity; i++) {
		vector<double> row(Ncity);
		fEdgeDis.push_back(row);
	}

	fNearCity.clear();
	for (int i = 0; i < Ncity; i++) {
		vector<int> row(fNearNumMax + 1);
		fNearCity.push_back(row);
	}

	if( strcmp( type, "EUC_2D" ) == 0  ) {
		//std::cout << fEdgeDis.size() << " " << fEdgeDis[0].size() << std::endl;
		for( int i = 0; i < Ncity ; ++i )
			for( int j = 0; j < Ncity ; ++j ){
				fEdgeDis[ i ][ j ]= sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
			}
	} else if ( strcmp( type, "ATT" ) == 0  ) {
		for( int i = 0; i < Ncity; ++i ){
			for( int j = 0; j < Ncity; ++j ) {
				double r = (sqrt(((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]))/10.0));
				fEdgeDis[ i ][ j ] = r;
			}
		}
	} else if (strcmp( type, "CEIL_2D" ) == 0) {
		for (int i = 0; i < Ncity ; ++i) {
			for (int j = 0; j < Ncity ; ++j) {
				fEdgeDis[ i ][ j ]=(int)ceil(sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
			}
		}
	} else {
		printf( "EDGE_WEIGHT_TYPE is not supported\n" );
		exit( 1 );
	}
	int ci, j1, j2, j3;
	int cityNum = 0;
	double minDis;
	for( ci = 0; ci < Ncity; ++ci ){
		for( j3 = 0; j3 < Ncity; ++j3 ) checkedN[ j3 ] = 0;
		checkedN[ ci ] = 1;
		fNearCity[ ci ][ 0 ] = ci;
		for( j1 = 1; j1 <= fNearNumMax; ++j1 ) {
			minDis = 100000000;
			for( j2 = 0; j2 < Ncity; ++j2 ){
				if( fEdgeDis[ ci ][ j2 ] <= minDis && checkedN[ j2 ] == 0 ){
					cityNum = j2;
					minDis = fEdgeDis[ ci ][ j2 ];
				}
			}
			fNearCity[ ci ][ j1 ] = cityNum;
			checkedN[ cityNum ] = 1;
		}
	}
}

void TEvaluator::doIt( TIndi& indi ) {
	double d = 0;
	for( int i = 0; i < Ncity; ++i ) d += fEdgeDis[ i ][ indi.fLink[i][0] ] + fEdgeDis[ i ][ indi.fLink[i][1] ];
	indi.fEvaluationValue = d / 2.0;
}

void TEvaluator::writeTo( FILE* fp, TIndi& indi ){
	Array.resize(Ncity);
	int curr=0, st=0, count=0, pre=-1, next;
	while( 1 ){
		Array[ count++ ] = curr + 1;
		if( count > Ncity ){
			printf( "Invalid\n" );
			return;
		}
		if( indi.fLink[ curr ][ 0 ] == pre ) next = indi.fLink[ curr ][ 1 ];
		else next = indi.fLink[ curr ][ 0 ];

		pre = curr;
		curr = next;
		if( curr == st ) break;
	}
	if( this->checkValid( Array, indi.fEvaluationValue ) == false )
		printf( "Individual is invalid \n" );

	fprintf( fp, "%d %f\n", indi.fN, indi.fEvaluationValue );
	for( int i = 0; i < indi.fN; ++i )
		fprintf( fp, "%d ", Array[ i ] );
	fprintf( fp, "\n" );
}

pair<double, vector<int>> TEvaluator::returnBestSolution(TIndi& indi){
	Array.resize(Ncity);
	int curr=0, st=0, count=0, pre=-1, next;
	while( 1 ){
		Array[ count++ ] = curr + 1;
		if( count > Ncity ){
			printf( "Invalid\n" );
			return {0, {}};
		}
		if( indi.fLink[ curr ][ 0 ] == pre ) next = indi.fLink[ curr ][ 1 ];
		else next = indi.fLink[ curr ][ 0 ];

		pre = curr;
		curr = next;
		if( curr == st ) break;
	}
	// if( this->checkValid( Array, indi.fEvaluationValue ) == false )
	// 	printf( "Individual is invalid \n" );
	// else
	// 	printf(" Individual is valid \n");
	vector<int> res;
	for( int i = 0; i < indi.fN; ++i )
		res.push_back(Array[i] - 1);
	
	res.push_back(Array[0] - 1);
	
	return {indi.fEvaluationValue, res};
}

bool TEvaluator::checkValid(vector<int>& array, double value) {
	int *check=new int[Ncity];
	for( int i = 0; i < Ncity; ++i ) check[ i ] = 0;
	for( int i = 0; i < Ncity; ++i ) ++check[ array[ i ]-1 ];
	for( int i = 0; i < Ncity; ++i )
		if( check[ i ] != 1 ) return false;
	double distance = 0;
	for( int i = 0; i < Ncity-1; ++i )
		distance += fEdgeDis[ array[ i ]-1 ][ array[ i+1 ]-1 ];

	distance += fEdgeDis[ array[ Ncity-1 ]-1 ][ array[ 0 ]-1 ];
	//printf( "Distance: %f, Value: %f\n", distance, value);
	delete [] check;
	if( abs(distance - value) > 1e-3 ) return false;
	return true;
}

