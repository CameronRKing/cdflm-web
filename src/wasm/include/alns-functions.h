#ifndef ALNS_FUNCS_H
#define ALNS_FUNCS_H

#include <map>
#include <vector>
#include <algorithm>
#include "defs.h"
#include "Utils.h"
#include "ALNSFunction.h"
#include "ALNSSolution.h"
using namespace std;

/**
 * Struct to hold a repair/destroy functional object pair
 **/
struct FuncPair {
    ALNSFunction* repair;
    ALNSFunction* destroy;
};

/**
 * Facility Destroy Functions
 **/

/**
 * Destroys a given number of facilities at random
 * Assumes that the passed-in solution is valid
 *
 * @param ALNSSolution solution
 * @return ALNSSolution destroyedSolution (i.e., a few entries are removed from the facilities vector)
 **/
class FacRandQDestroy : public ALNSFunction {
public:
    FacRandQDestroy(int q) : ALNSFunction(q) {};
    ALNSSolution operator()(ALNSSolution solution) {
        this->timesUsed++;

        // destroy q facilities at random
        for (int i = 0; i < this->numToChange; i++) {
            int randNum = rand() % solution.facilities.size();
            solution.facilities.erase(solution.facilities.begin() + randNum);
            solution.numUnassigned++;
        }
        return solution;
    }
};

/**
 * Destroys q facilities with the worst objective measure, whatever that might be
 * Assumes that the passed-in solution is valid
 *
 * @param ALNSSolution solution
 * @return ALNSSolution destroyedSolution (i.e., a few entries are removed from the facilities vector)
 **/
class FacWorstQDestroy : public ALNSFunction {
public:
    FacWorstQDestroy(int q) : ALNSFunction(q) {};
    ALNSSolution operator()(ALNSSolution solution) {
        this->timesUsed++;
        solution.sortFacsByMeasures();

        // destroy q facilities with the worst fitness
        // facilities are sorted ascending, so worst are at the end
        int length = solution.facilities.size() - 1;
        for (int i = length; i > length - this->numToChange; i--) {
            solution.facilities.erase(solution.facilities.begin() + i);
            solution.numUnassigned++;
        }
        return solution;
    }
};

/**
 * Destroys q facilities with the best objective measure, whatever that might be
 * Assumes that the passed-in solution is valid
 *
 * @param ALNSSolution solution
 * @return ALNSSolution destroyedSolution (i.e., a few entries are removed from the facilities vector)
 **/
class FacBestQDestroy : public ALNSFunction {
public:
    FacBestQDestroy(int q) : ALNSFunction(q) {};
    ALNSSolution operator()(ALNSSolution solution) {
        this->timesUsed++;
        // get and sort appropriate measures (stars/radii) in descending order
        solution.sortFacsByMeasures();

        // destroy q facilities with best fitness
        // we sorted in ascending order, so best fitness is at the beginning
        for (int i = 0; i < this->numToChange; i++) {
            solution.facilities.erase(solution.facilities.begin());
            solution.numUnassigned++;
        }
        return solution;
    }
};




/**
 * Facility Repair Functions
 **/

/**
 * Allocates new facilities at random
 * Assumes that the passed-in solution is NOT valid and needs to be repaired
 *
 * @param ALNSSolution solution
 * @param ALNSSolution repairedSolution (i.e., with a few more entries in the facilities vector)
 **/
class FacRandRepair : public ALNSFunction {
public:
    ALNSSolution operator()(ALNSSolution solution) {
        this->timesUsed++;
        int numUnassigned = solution.numUnassigned;
        int possibleFacs  = solution.data.numCustomers;

        // install necessary number of new facilities at random
        for (int i = 0; i < numUnassigned; i++) {
            int fac;
            vector<int>::iterator it;
            do {
                fac = rand() % possibleFacs;
                it = find(solution.facilities.begin(), solution.facilities.end(), fac);
            } while (it != solution.facilities.end());
            solution.facilities.push_back(fac);
            solution.numUnassigned--;
        }
        solution.update();
        return solution;
    }
};

/**
 * Allocates new facilities by performing a local search
 * Assumes that the passed-in solution is NOT valid and needs to be repaired
 *
 * ???HOW ARE WE GOING TO DO A LOCAL SEARCH???
 *
 * @param ALNSSolution solution
 * @param ALNSSolution repairedSolution (i.e., with a few more entries in the facilities vector)
 **/
class FacLSRepair : public ALNSFunction {
public:
    ALNSSolution operator()(ALNSSolution solution) {
        timesUsed++;
        FacRandRepair rep = FacRandRepair();
        return rep(solution);
    }
};

#endif