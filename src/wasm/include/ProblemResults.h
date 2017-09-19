#ifndef PROBLEMRESULTS_H
#define PROBLEMRESULTS_H

#include "defs.h"
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// structure for holding the results of an attempted optimization
struct ProblemResults {
    /* members */
    float time;
    int objective;                      // by definition, our problems use only integers
    vector<int> facilities;             // which facilities are currently open?
    vector<int> customerAssignments;    // to which facility is a customer assigned?
    ProblemType type;
    ObjectiveType objType;

    /* functions */
    string getJSONFacilities() {
        string json = "[";
        int length = facilities.size();
        for (int i = 0; i < length; i++) {
            json += to_string(facilities[i]);
            if (i < (length - 1)) {
                json += ", ";
            }
        }
        json += "]";
        return json;
    }

    string getJSONCustomers() {
        string json = "[";
        int length = customerAssignments.size();
        for (int i = 0; i < length; i++) {
            json += to_string(customerAssignments[i]);
            if (i < (length - 1)) {
                json += ", ";
            }
        }
        json += "]";
        return json;
    }

    string getHash() {
        string hash = "";   // since order doesn't matter, we need to sort the facilities to make sure that
        sort(facilities.begin(), facilities.end()); // solutions with different orders of the same facilities get hashed the same
        for (int fac : facilities) { hash += to_string(fac); }
        return hash;
    }
};

#endif
