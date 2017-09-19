#ifndef ALNSFUNCTION_H
#define ALNSFUNCTION_H

#include <map>
#include <vector>
#include "defs.h"
#include "Utils.h"
#include "ALNSSolution.h"
using namespace std;

/**
 * Interface for our repair/destroy functions
 * Rather than use function pointers, it seems cleaner to use a class hierarchy of functional objects
 **/
class ALNSFunction {
public:
    ALNSFunction() { this->numToChange = 1; this->score = 0.0; this->timesUsed = 0; }
    ALNSFunction(int q) : ALNSFunction() { this->numToChange = q; }
    virtual ALNSSolution operator()(ALNSSolution) = 0;

    void setNumToChange(int q) { this->numToChange = q; }
    int  getNumToChange() { return this->numToChange; }

    void  addToScore(float addition) { this->score += addition; }
    float getScore() { return this->score; }

    int getTimesUsed() { return this->timesUsed; }

    void reset() { this->score = 0.0; this->timesUsed = 0; }
    
protected:
    int   numToChange;
    float score;
    int timesUsed = 0;
};

#endif