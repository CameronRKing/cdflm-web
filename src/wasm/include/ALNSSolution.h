#ifndef ALNSSOLUTION_H
#define ALNSSOLUTION_H

#include <map>
#include <vector>
#include <algorithm>
#include "ProblemData.h"
using namespace std;

class ALNSSolution {
public:
    /* members */
    ProblemData data;
    int objective;
    vector<int> facilities;
    vector<int> customerAssignments;
    int numUnassigned;

    /* functions */
    string getJSONFacilities();
    string getJSONCustomers();
    string getHash();
    map<int, int> getMeasures();
    void sortFacsByMeasures();
    map<int, int> getStars();
    map<int, int> getRadii();
    map<int, int> getRays();
    void update();
};

#endif