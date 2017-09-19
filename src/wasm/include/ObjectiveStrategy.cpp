#include "ObjectiveStrategy.h"

#include <map>
#include <vector>
#include <algorithm>
#include "defs.h"
using namespace std;

/**
 * Conditional logic for calculating appropriate objective
 * based on problem type.
 *
 * @param const vector<vector<int>> costs,
 * @param const vector<int> customerAssignments,
 * @param const ProblemType problemType)
 * @return int objective
 **/
int ObjectiveStrategy::calcObjective(const vector<vector<int>> costs, 
                                     const vector<int> customerAssignments,
                                     const ProblemType problemType) {
    switch (problemType) {
        case MAX_STAR:
            return ObjectiveStrategy::calcMaxStar(costs, customerAssignments);
        case SUM_STAR:
            return ObjectiveStrategy::calcSumStar(costs, customerAssignments);
        case MIN_STAR:
            return ObjectiveStrategy::calcMinStar(costs, customerAssignments);
        case MAX_RADIUS:
            return ObjectiveStrategy::calcMaxRadius(costs, customerAssignments);
        case SUM_RADIUS:
            return ObjectiveStrategy::calcSumRadius(costs, customerAssignments);
        case MIN_RADIUS:
            return ObjectiveStrategy::calcMinRadius(costs, customerAssignments);
        case MAX_RAY:
            return ObjectiveStrategy::calcMaxRay(costs, customerAssignments);
        case SUM_RAY:
            return ObjectiveStrategy::calcSumRay(costs, customerAssignments);
        case MIN_RAY:
            return ObjectiveStrategy::calcMinRay(costs, customerAssignments);
        default:
            throw "Unsupported problem type!";
    }
}

/***********************/
/*                     */
/*        STARS        */
/*                     */
/***********************/
int ObjectiveStrategy::calcMaxStar(const vector<vector<int>> costs, 
                                   const vector<int> customerAssignments) {
    map<int, int> stars;
    int fac;
    for (int cust = 0; cust < customerAssignments.size(); cust++) {
        fac = customerAssignments[cust];
        if (stars.count(fac) == 0) {
            stars[fac] = 0;
        }
        stars[fac] += costs[cust][fac];
    }

    pair<int, int> maxStar = *max_element(stars.begin(), stars.end(),
                                         [](pair<int, int> left, pair<int, int> right) { 
                                             return left.second < right.second; 
                                         });
    return maxStar.second;
}

int ObjectiveStrategy::calcSumStar(const vector<vector<int>> costs, 
                                   const vector<int> customerAssignments) {
    int sumStar = 0;
    int fac;
    for (int cust = 0; cust < customerAssignments.size(); cust++) {
        fac = customerAssignments[cust];
        sumStar += costs[cust][fac];
    }
    return sumStar;
}

int ObjectiveStrategy::calcMinStar(const vector<vector<int>> costs, 
                                   const vector<int> customerAssignments) {
    map<int, int> stars;
    int fac;
    for (int cust = 0; cust < customerAssignments.size(); cust++) {
        fac = customerAssignments[cust];
        if (stars.count(fac) == 0) {
            stars[fac] = 0;
        }
        stars[fac] += costs[cust][fac];
    }

    pair<int, int> minStar = *min_element(stars.begin(), stars.end(),
                                         [](pair<int, int> left, pair<int, int> right) { 
                                             return left.second < right.second; 
                                         });
    return minStar.second;
}


/*********************/
/*                   */
/*       RADII       */
/*                   */
/*********************/
int ObjectiveStrategy::calcMaxRadius(const vector<vector<int>> costs, 
                                     const vector<int> customerAssignments) {
    map<int, int> radii;
    int fac;
    for (int cust = 0; cust < customerAssignments.size(); cust++) {
        fac = customerAssignments[cust];
        if (radii.count(fac) == 0 || costs[cust][fac] > radii[fac]) {
            radii[fac] = costs[cust][fac];
        }
    }

    pair<int, int> maxRadius = *max_element(radii.begin(), radii.end(),
                                            [](pair<int, int> left, pair<int, int> right) { 
                                                return left.second < right.second; 
                                            });
    return maxRadius.second;
}

int ObjectiveStrategy::calcSumRadius(const vector<vector<int>> costs, 
                                     const vector<int> customerAssignments) {
    map<int, int> radii;
    int fac;
    for (int cust = 0; cust < customerAssignments.size(); cust++) {
        fac = customerAssignments[cust];
        if (radii.count(fac) == 0 || costs[cust][fac] > radii[fac]) {
            radii[fac] = costs[cust][fac];
        }
    }

    int sumRadius = 0;
    for (auto pair : radii) {
        sumRadius += pair.second;
    }
    return sumRadius;
}

int ObjectiveStrategy::calcMinRadius(const vector<vector<int>> costs, 
                                     const vector<int> customerAssignments) {
    map<int, int> radii;
    int fac;
    for (int cust = 0; cust < customerAssignments.size(); cust++) {
        fac = customerAssignments[cust];
        if (radii.count(fac) == 0 || costs[cust][fac] > radii[fac]) {
            radii[fac] = costs[cust][fac];
        }
    }

    pair<int, int> minRadius = *min_element(radii.begin(), radii.end(),
                                            [](pair<int, int> left, pair<int, int> right) { 
                                                return left.second < right.second; 
                                            });
    return minRadius.second;
}

/**********************/
/*                    */
/*        RAYS        */
/*                    */
/**********************/
int ObjectiveStrategy::calcMaxRay(const vector<vector<int>> costs,
                                  const vector<int> customerAssignments) {
    map<int, int> rays;
    int fac;
    for (int cust = 0; cust < customerAssignments.size(); cust++) {
        fac = customerAssignments[cust];
        if (rays.count(fac) == 0 || costs[cust][fac] < rays[fac]) {
            rays[fac] = costs[cust][fac];
        }
    }

    pair<int, int> maxRay = *max_element(rays.begin(), rays.end(),
                                            [](pair<int, int> left, pair<int, int> right) { 
                                                return left.second < right.second; 
                                            });
    return maxRay.second;
}

int ObjectiveStrategy::calcSumRay(const vector<vector<int>> costs,
                                  const vector<int> customerAssignments) {
    map<int, int> rays;
    int fac;
    for (int cust = 0; cust < customerAssignments.size(); cust++) {
        fac = customerAssignments[cust];
        if (rays.count(fac) == 0 || costs[cust][fac] < rays[fac]) {
            rays[fac] = costs[cust][fac];
        }
    }

    int sumRay = 0;
    for (auto pair : rays) {
        sumRay += pair.second;
    }
    return sumRay;
}

int ObjectiveStrategy::calcMinRay(const vector<vector<int>> costs,
                                  const vector<int> customerAssignments) {
    map<int, int> rays;
    int fac;
    for (int cust = 0; cust < customerAssignments.size(); cust++) {
        fac = customerAssignments[cust];
        if (rays.count(fac) == 0 || costs[cust][fac] < rays[fac]) {
            rays[fac] = costs[cust][fac];
        }
    }

    pair<int, int> minRay = *min_element(rays.begin(), rays.end(),
                                            [](pair<int, int> left, pair<int, int> right) { 
                                                return left.second < right.second; 
                                            });
    return minRay.second;
}
