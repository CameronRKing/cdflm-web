#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <vector>
#include <string>
#include "Algorithm.h"
#include "ProblemData.h"
#include "ProblemResults.h"
using namespace std;

namespace Utils {
    vector<string> split(string, string);
	ProblemData getData(string);
	ProblemData parseORLIB(string);
	ProblemData parseDaskin(string);
    int calcCost(const vector<float>&, const vector<float>&);
	void printMatrix(const vector<vector<int>>&);
    void printVector(const vector<int>&);
    void optimizeForEachProblemType(Algorithm*, ProblemData);
}

#endif
