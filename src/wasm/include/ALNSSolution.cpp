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
 * Sorts facilities vector by appropriate measures
 * 
 * @return void
 **/
void ALNSSolution::sortFacsByMeasures() {
    // turn the measures map into a vector of pairs
    map<int, int> measures = this->data.getMeasures(this->customerAssignments);
    vector<pair<int, int>> pairs;
    for (auto itr = measures.begin(); itr != measures.end(); ++itr) {
        pairs.push_back(*itr);
    }
    // sort the vector of pairs in ascending order
    Comparator comparator(this->data.type.objective);
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
 * Updates customer assignments and objective
 *
 * @return void
 **/
void ALNSSolution::update() {
    this->customerAssignments = this->data.assignCustomers(this->facilities);
    this->objective = this->data.calcObjective(this->customerAssignments);
}