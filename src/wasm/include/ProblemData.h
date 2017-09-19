#ifndef PROBLEMDATA_H
#define PROBLEMDATA_H

#include "defs.h"
#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// structure for holding data about the problem
struct ProblemData {
    string name;
    ProblemType type;
    int numFacilities;
    int numCustomers;
    vector<vector<int>> costs;
    vector<vector<int>> demand;

    /**
     * Calculates objective value for given customer assignments
     *
     * @param const vector<int> assignments,
     * @return int objective
     **/
    int calcObjective(const vector<int> assignments) {
        map<int, int> measures = this->getMeasures(assignments);
        return this->getAggregate(measures);
    }

    map<int, int> getMeasures(const vector<int> assignments) {
        switch (this->type.measure) {
            case STAR:
                return this->calcStars(assignments);
            case RADIUS:
                return this->calcRadii(assignments);
            case RAY:
                return this->calcRays(assignments);
            default:
                throw "Unsupported problem type!";
        }
    }

    int getAggregate(const map<int, int> measures) {
        switch (this->type.aggregate) {
            case MAX:
                return this->getMax(measures);
            case MIN:
                return this->getMin(measures);
            case SUM:
                return this->getSum(measures);
            default:
                throw "Unsupported problem type!";
        }
    }

    map<int, int> calcStars(const vector<int> assignments) {
        map<int, int> stars;
        for (int cust = 0; cust < assignments.size(); cust++) {
            int fac = assignments[cust];
            if (stars.count(fac) == 0) {
                stars[fac] = 0;
            }
            stars[fac] += this->costs[cust][fac];
        }
        return stars;
    }

    map<int, int> calcRadii(const vector<int> assignments) {
        map<int, int> radii;
        for (int cust = 0; cust < assignments.size(); cust++) {
            int fac = assignments[cust];
            if (radii.count(fac) == 0 || costs[cust][fac] > radii[fac]) {
                radii[fac] = this->costs[cust][fac];
            }
        }
        return radii;
    }

    map<int, int> calcRays(const vector<int> assignments) {
        map<int, int> rays;
        for (int cust = 0; cust < assignments.size(); cust++) {
            int fac = assignments[cust];
            if (rays.count(fac) == 0 || costs[cust][fac] < rays[fac]) {
                rays[fac] = costs[cust][fac];
            }
        }
        return rays;
    }

    int getMax(const map<int, int> measures) {
        return max_element(measures.begin(), measures.end(),
            [](pair<int, int> left, pair<int, int> right) { 
                return left.second < right.second; 
            })->second;
    }

    int getMin(const map<int, int> measures) {
        return min_element(measures.begin(), measures.end(),
            [](pair<int, int> left, pair<int, int> right) { 
                return left.second < right.second; 
            })->second;
    }

    int getSum(const map<int, int> measures) {
        int sum = 0;
        for (auto pair : measures) {
            sum += pair.second;
        }
        return sum;
    }

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