#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include "defs.h"
#include "Listener.h"
#include "Comparator.h"
#include "ProblemData.h"
#include "ProblemResults.h"
#include "ObjectiveStrategy.h"
using namespace std;

class Logger;

/**
 * Abstract class that defines contract for any potential algorithms to implement
 **/
class Algorithm {
public:
    virtual ~Algorithm() {};
    virtual ProblemResults optimize(ProblemData) = 0;
    // should remove these calls entirely, I think
    virtual int calcObjective(const vector<int>& customerAssignments) { 
        return ObjectiveStrategy::calcObjective(this->data.costs, customerAssignments, this->data.type); 
    };
    virtual string getName() = 0;
    virtual string getJSONParameters() = 0;
    void      setListener(Listener* l) { this->listener = l; };
    Listener* getListener() { return this->listener; };
protected:
    ProblemData data;
    Listener* listener;
    Comparator comparator;
};

#endif