#include "ALNSSolution.h"

#include <map>
#include <vector>
#include "Comparator.h"
#include "ProblemData.h"
using namespace std;


string ALNSSolution::getJSONFacilities() {
    string json = "[";
    for (int fac : facilities) {
        json += to_string(fac) + ", ";
    }
    json += "]";
    return json;
}

string ALNSSolution::getJSONCustomers() {
    string json = "[";
    for (int cust : customerAssignments) {
        json += to_string(cust) + ", ";
    }
    json += "]";
    return json;
}

string ALNSSolution::getHash() {
    string hash = "";                           // since order doesn't matter, we need to sort the facilities to make sure that
    sort(facilities.begin(), facilities.end()); // solutions with different orders of the same facilities get hashed the same
    for (int fac : facilities) { hash += to_string(fac); }
    return hash;
}

/**
 * Returns appropriate measures based on problem type
 *
 * @return map<int, int>: <facility, measure>
 **/
map<int, int> ALNSSolution::getMeasures() { 
    switch (this->data.type) {
        case SUM_STAR:
        case MAX_STAR:
        case MIN_STAR:
            return this->getStars();
            break;
        case SUM_RADIUS:
        case MAX_RADIUS:
        case MIN_RADIUS:
            return this->getRadii();
            break;
        case SUM_RAY:
        case MAX_RAY:
        case MIN_RAY:
            return this->getRays();
            break;
        default:
            throw "Unrecognized problem type!";
        break;
    }
}

/**
 * Sorts facilities vector by appropriate measures
 * 
 * @return void
 **/
void ALNSSolution::sortFacsByMeasures() {
    // turn the measures map into a vector of pairs
    map<int, int> measures = this->getMeasures();
    vector<pair<int, int>> pairs;
    for (auto itr = measures.begin(); itr != measures.end(); ++itr) {
        pairs.push_back(*itr);
    }
    // sort the vector of pairs in ascending order
    Comparator comparator(this->data.objType);
    sort(pairs.begin(), pairs.end(), [&](std::pair<int, int> a, std::pair<int, int> b)
        {
            return comparator(a.second, b.second);
        }
    );
    // copy the new order into the facilities vector
    int i = 0;
    for (auto pair : pairs) {
        this->facilities[i++] = pair.first;
    }
}

/**
 * Returns map of max stars for current solution
 * Does NOT assign customers --> only calculates stars
 *
 * @return map<int, int> <facility, star>
 **/
map<int, int> ALNSSolution::getStars() {
    map<int, int> stars;
    for (int i = 0; i < this->customerAssignments.size(); i++) {
        int fac = this->customerAssignments[i];
        if (stars.count(fac) == 0) {
            stars[fac] = 0;
        }
        stars[fac] += this->data.costs[i][fac];
    }
    return stars;
}

/**
 * Returns map of radii of facilities
 *
 * @return map<int, int> <facility, radius>
 **/
map<int, int> ALNSSolution::getRadii() {
    map<int, int> radii;
    for (int i = 0; i < this->customerAssignments.size(); i++) {
        int fac = this->customerAssignments[i];
        if (radii.count(fac) == 0) {
            radii[fac] = 0;
        }
        if (this->data.costs[i][fac] > radii[fac]) {
            radii[fac] = this->data.costs[i][fac];
        } 
    }
    return radii;
}

/**
 * Returns map of rays (shortest customer distance) of facilities
 *
 * @return map<int, int> <facility, ray>
 **/
map<int, int> ALNSSolution::getRays() {
    map<int, int> rays;
    for (int i = 0; i < this->customerAssignments.size(); i++) {
        int fac = this->customerAssignments[i];
        if (rays.count(fac) == 0 || this->data.costs[i][fac] < rays[fac]) {
            rays[fac] = this->data.costs[i][fac];
        }
    }
    return rays;
}

/**
 * Updates customer assignments and objective
 *
 * @return void
 **/
void ALNSSolution::update() {
    this->customerAssignments = this->data.assignCustomers(this->facilities);
    this->objective = this->data.calcObjective(this->customerAssignments);
}