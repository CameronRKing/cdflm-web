#ifndef PROBLEMDATA_H
#define PROBLEMDATA_H

#include "defs.h"
#include <vector>
#include <string>
using namespace std;

// structure for holding data about the problem
struct ProblemData {
    string name;
    ProblemType type;
    ObjectiveType objType;
    int numFacilities;
    int numCustomers;
    vector<vector<int>> costs;
    vector<vector<int>> demand;
};

#endif