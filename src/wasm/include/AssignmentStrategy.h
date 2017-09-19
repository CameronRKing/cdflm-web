#ifndef ASSIGNMENT_STRATEGY_H
#define ASSIGNMENT_STRATEGY_H

#include "defs.h"
#include <vector>
#include <map>
using namespace std;
// since we usually assign customers to the closest facility,
// but I want to explore using alternative methods, this class
// provides a convenient way to encapsulate those strategies.
// also makes it easier to access the same algorithms from different places.
class AssignmentStrategy {
public:
    static vector<int> assign(const vector<vector<int>>, const vector<int>, const ProblemType);
    static vector<int> closestFacility(const vector<vector<int>>, const vector<int>);
    static vector<int> greedyMaxStarHeuristic(const vector<vector<int>>, const vector<int>);
private:
    static map<int, vector<int>> getFacilityAssignments(const vector<int>);
    static int getNextClosestFacility(const vector<int>, const pair<int, int>, const vector<int>);
    static map<int, int> getStars(const vector<vector<int>>, const map<int, vector<int>>);
    static pair<int, int> getMaxStar(const map<int, int>);
};

#endif