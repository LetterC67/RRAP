#ifndef RUN_TSP_H
#define RUN_TSP_H

#include <vector>
#include <utility>

std::pair<double, std::vector<int>> run(int N, std::vector<std::vector<double>> &distances);

#endif