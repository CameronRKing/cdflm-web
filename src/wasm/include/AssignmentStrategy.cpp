#include "AssignmentStrategy.h"

#include "defs.h"
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

vector<int> AssignmentStrategy::assign(const vector<vector<int>> costs,
                                       const vector<int> facilities,
                                       const ProblemType type) {
    
    // let's enforce closest facility assignments for now, since it's likely
    // that nobody will like my greedy assignment algorithm
    return AssignmentStrategy::closestFacility(costs, facilities);
    // switch (type) {
    //     case MAX_STAR:
    //         return AssignmentStrategy::greedyMaxStarHeuristic(costs, facilities);
    //     default:
    //         return AssignmentStrategy::closestFacility(costs, facilities);
    // }
}

/**
 * Assigns customers to their closest facility
 *
 * @param const vector<vector<int>> costs
 * @param const vector<int> facilities
 * @return vector<int> customerAssignments
 **/
vector<int> AssignmentStrategy::closestFacility(const vector<vector<int>> costs, const vector<int> facilities) {
    vector<int> customerAssignments;

    int localMin = 0;
    int facility = 0;
    for (int cust = 0; cust < costs.size(); cust++) {
        // bootstrap our search
        facility = facilities[0];
        localMin = costs[cust][facility];
        for (int fac = 1; fac < facilities.size(); fac++) {
            // keep going if we can't do better than what we've already got
            if (localMin <= costs[cust][facilities[fac]]) continue;

            localMin = costs[cust][facilities[fac]];
            facility = facilities[fac];
        }

        // set customer/facility assignment
        customerAssignments.push_back(facility);
    }

    return customerAssignments;
}

/**
 * 
 * @
 **/
vector<int> AssignmentStrategy::greedyMaxStarHeuristic(const vector<vector<int>> costs, const vector<int> facilities) {
    // leverage another strategy for our starting point
              vector<int> customerAssignments = AssignmentStrategy::closestFacility(costs, facilities);
    map<int, vector<int>> facilityAssignments = AssignmentStrategy::getFacilityAssignments(customerAssignments);
            map<int, int> stars = AssignmentStrategy::getStars(costs, facilityAssignments);
    
    pair<int, int> maxStar = AssignmentStrategy::getMaxStar(stars);
    pair<int, int> potentialFac; // the facility to which we might assign our max star's most expensive customer
    int maxStarCost;             // how much it costs to assign the expensive customer to the current max star
    int potentialCost;           // how much it costs to assign the expensive customer to its next-closest facility
    int expensiveCust;           // the customer number of the max star's most expensive customer
    int customerPos;             // tracks the position of the most expensive customer in the facilityAssignments[maxStarFac] vector for deletion later
    int iterations = 0;          // number of times we're run through the assignment loop
    int failsafe   = 1000;       // an upper bound on our assignment loop in case something goes wrong
    bool tryAgain  = true;       // should we run through the assignment loop again?

    // bootstrap our heuristic by sorting the customers at the current max star by their costs 
    // and setting the most expensive customer
    vector<int> costRow = costs[maxStar.first];
    sort(facilityAssignments[maxStar.first].begin(),
         facilityAssignments[maxStar.first].end(),
         [&costRow](int left, int right) { 
             return costRow[left] > costRow[right]; 
         });
    expensiveCust = facilityAssignments[maxStar.first][0];
    customerPos   = 0;
    maxStarCost   = costRow[expensiveCust];

    do {
        
        // find most expensive customer's next-closest facility
        int nextClosest = AssignmentStrategy::getNextClosestFacility(costRow, maxStar, facilities);
        potentialFac    = *(stars.find(nextClosest));
        potentialCost   = costs[potentialFac.first][expensiveCust];

        // if it's better for us to move the customer, then reassign and recalculate max star
        if (potentialFac.second + potentialCost < maxStar.second) {
            // update star values
            potentialFac.second += potentialCost;
            maxStar.second      -= maxStarCost;
            // not sure if modifying the pair modifies the map
            // so I'm going to explicitly save back into the map
            stars[potentialFac.first] = potentialFac.second;
            stars[maxStar.first]      = maxStar.second;

            // reassign customer
            customerAssignments[expensiveCust] = potentialFac.first;
            facilityAssignments[potentialFac.first].push_back(expensiveCust);
            facilityAssignments[maxStar.first].erase(facilityAssignments[maxStar.first].begin() + customerPos);

            // update max star
            int newFac = AssignmentStrategy::getMaxStar(stars).first;
            // if we've switch facilities, reset all the relevant data
            if (newFac != maxStar.first) {
                maxStar = *(stars.find(newFac));
                costRow = costs[maxStar.first];
                sort(facilityAssignments[maxStar.first].begin(),
                     facilityAssignments[maxStar.first].end(),
                     [&costRow](int left, int right) { 
                         return costRow[left] > costRow[right]; 
                     });
                customerPos = 0;
            }
            // the max star did not change
            else {
                // if we've tried to reassign all customers at this facility, the heuristic is done
                if (customerPos == facilityAssignments[maxStar.first].size()) {    // we use .size() instead of .size() - 1 because of the edge case where we just reassigned the last customer
                    tryAgain = false;                                              // e.g., before reassignment: customerPos = 12; vector.size() = 13
                }                                                                  //        after reassignment: customerPos = 12; vector.size() = 12
                // don't update customerPos. Since our customers are already sorted by cost,
                // and we just removed one, customerPos already points at the next most expensive
            }
            // reset the customer max star values
            expensiveCust  = facilityAssignments[maxStar.first][customerPos];
            maxStarCost    = costs[maxStar.first][expensiveCust];
            maxStar.second = stars[maxStar.first];
        }
         // if the potential assignment was NOT better 
        else {
            // if we've tried to reassign all customers at this facility, the heuristic is done
            if (customerPos == facilityAssignments[maxStar.first].size() - 1) {
                tryAgain = false;
            } 
            // if we've got customers left to try to reassign, let's move to the next one
            else {
                ++customerPos;
                expensiveCust = facilityAssignments[maxStar.first][customerPos];
                maxStarCost   = costs[maxStar.first][expensiveCust];
            }
        }
    } while (tryAgain && ++iterations < failsafe);

    if (iterations == failsafe) {
        cout << endl << "AssignmentStrategy::greedyMaxStarHeuristic(): iterations reached failsafe! Step through your code and make sure nothing went wrong!" << endl;
    }

    return customerAssignments;

}

/**
 * Returns a map of the customers assigned to a given facility, indexed by facility number
 * Doesn't do any calculation. Just rolls up our data in a differen way.
 *
 * @param const vector<int> customerAssignments
 * @param const map<int, vector<int>> facilityAssignments
 **/
map<int, vector<int>> AssignmentStrategy::getFacilityAssignments(const vector<int> customerAssignments) {
    map<int, vector<int>> facilityAssignments;

    for (int cust = 0; cust < customerAssignments.size(); cust++) {
        int fac = customerAssignments[cust];
        if (facilityAssignments.count(fac) == 0) {
            facilityAssignments[fac] = vector<int> {cust};
        } else {
            facilityAssignments[fac].push_back(cust);
        }
    }
    return facilityAssignments;
}

/**
 * Calculates star values.
 * Note that we take facility assignments, not customer assignments
 * This isn't by necessity, but because it makes the function easier to write
 * and a little more straightforward.
 *
 * @param const vector<vector<int>> costs,
 * @param const map<int, vector<int>> facilityAssignments
 * @return map<int, int> stars
 **/
map<int, int> AssignmentStrategy::getStars(const vector<vector<int>> costs, 
                                           const map<int,vector<int>> facilityAssignments) {
    map<int, int> stars;

    for (auto pair: facilityAssignments) {
        int fac = pair.first;
        int star = 0;
        for (int cust : pair.second) {
            star += costs[cust][fac];
        }
        stars[fac] = star;
    }

    return stars;
}

/**
 * Helper function to reduce complexity of greedyMaxStarHeuristic()
 *
 * @param const map<int, int> stars
 * @return pair<int, int> star
 **/
pair<int, int> AssignmentStrategy::getMaxStar(const map<int, int> stars) {
    return *max_element(stars.begin(), stars.end(), 
                       [](pair<int, int> left, pair<int, int> right) { 
                           return left.second < right.second;
                       });
}

/**
 * Helper function to get the next closest facility.
 * Have to pass in the current closest facility so we ignore it safely.
 * No guarantee that the current facility we're assigned to is the overall closest
 *     --this customer may have been reassigned several times.
 * @
 **/
int AssignmentStrategy::getNextClosestFacility(const vector<int> costRow,
                                               const pair<int, int> maxStar, 
                                               const vector<int> facilities) {
    int potentialFac, potentialCost;
    // bootstrap our assignment search
    if (facilities[0] == maxStar.first) {  // we want to ignore the current cheapest facility (the max star)
        potentialFac = facilities[1];
    } else {
        potentialFac = facilities[0];
    }
    potentialCost = costRow[potentialFac];
    for (int j = 1; j < facilities.size(); j++) {   // for each potential facility
        // keep going if we can't do better than what we've already got OR we're looking at the currently assigned facility
        if (potentialCost <= costRow[facilities[j]] || facilities[j] == maxStar.first) continue;
        
        potentialFac  = facilities[j];
        potentialCost = costRow[potentialFac];
    }

    return potentialFac;
}