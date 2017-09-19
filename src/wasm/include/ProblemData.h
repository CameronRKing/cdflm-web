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

    /**
     * Assigns customers to their closest facility
     *
     * @param const vector<int> facilities
     * @return vector<int> customerAssignments
     **/
    vector<int> assignCustomers(const vector<int> facilities) {
        vector<int> customerAssignments;
        for (int cust = 0; cust < this->costs.size(); cust++) {
            int bestFac  = facilities[0];
            int bestCost = this->costs[cust][bestFac];
            for (int facIdx = 1; facIdx < facilities.size(); facIdx++) {
                int fac = facilities[facIdx];
                int newCost = this->costs[cust][fac];
                if (newCost < bestCost) {
                    bestCost = newCost;
                    bestFac  = fac; 
                }
            }
            customerAssignments.push_back(bestFac);
        }
        return customerAssignments;
    }
};

#endif