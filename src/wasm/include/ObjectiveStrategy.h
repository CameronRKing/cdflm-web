#ifndef OBJECTIVE_STRATEGY_H
#define OBJECTIVE_STRATEGY_H

#include <vector>
#include "defs.h"
using namespace std;

// a class to contain all our methods for calculating objective values
// this way we can access these methods in various places without having
// to worry about inheritance hierarchies or encapsulation or anything like that
//
// This class comprises one of two points for adding new objectives.
// The other point is the ProblemType enum in defs.h
class ObjectiveStrategy {
public:
    static int calcObjective(const vector<vector<int>>, const vector<int>, const ProblemType);
    static int calcMaxStar(const vector<vector<int>>, const vector<int>);
    static int calcSumStar(const vector<vector<int>>, const vector<int>);
    static int calcMinStar(const vector<vector<int>>, const vector<int>);
    static int calcMaxRadius(const vector<vector<int>>, const vector<int>);
    static int calcSumRadius(const vector<vector<int>>, const vector<int>);
    static int calcMinRadius(const vector<vector<int>>, const vector<int>);
    static int calcMaxRay(const vector<vector<int>>, const vector<int>);
    static int calcSumRay(const vector<vector<int>>, const vector<int>);
    static int calcMinRay(const vector<vector<int>>, const vector<int>);
};

#endif