#ifndef ALNS_H
#define ALNS_H

#include <map>
#include <vector>
#include "Algorithm.h"
#include "ALNSSolution.h"
#include "ALNSFunction.h"
#include "alns-functions.h"
using namespace std;

// default values for parameters
const int   ALNS_MAX_ITERS  = 25000;
const int   SEGMENT_LENGTH  = 100;
const float REACTION_FACTOR = 0.8;
const float COOLING_FACTOR  = 0.99985;
const float START_TEMP_CTRL = 0.4;



class ALNS : public Algorithm {
public:
    ALNS();                    // what constructors might I need? What would I want to pass in?
    ~ALNS();
    ProblemResults optimize(ProblemData);
    string getName() { return "ALNS"; }
    string getJSONParameters();


    void setStartTempCtrl(float val) { this->startTempCtrl = val; }
    float getStartTempCtrl() { return this->startTempCtrl; }

    void setReactionFactor(float val) { this->reactionFactor = val; }
    float getReactionFactor() { return this->reactionFactor; }

    void setCoolingFactor(float val) { this->coolingFactor = val; }
    float getCoolingFactor() { return this->coolingFactor; }

    void setNewBestReward(float val) { this->outcomeScores[1] = val; }
    float getNewBestReward() { return this->outcomeScores[1]; }

    void setAcceptedBetterReward(float val) { this->outcomeScores[2] = val; }
    float getAcceptedBetterReward() { return this->outcomeScores[2]; }
    
    void setAcceptedWorseReward(float val) { this->outcomeScores[3] = val; }
    float getAcceptedWorseReward() { return this->outcomeScores[3]; }
private:
    /**
     * Helper Functions
     **/
    void gridSearch(float, float, float, void (ALNS::*)(float));
    void initDefaultFuncs();
    void resetFuncFitnesses();
    ALNSSolution generateInitialSolution();
    void calcStartingTemp(ALNSSolution);
    FuncPair selectFuncs();
    bool accept(ALNSSolution&, ALNSSolution&);
    void updateFuncFitnesses();

    /**
     * Parameters
     **/
    int   maxIterations;
    int   segmentLength;
    float reactionFactor;
    float coolingFactor;
    float startTempCtrl;
    float temperature;
    float outcomeScores[4] = {0.0, 3.0, 15.0, 24.0};

    /**
     * Working data
     **/
    map<string, bool> visited;
    map<ALNSFunction*, float> destroyFuncs;
    map<ALNSFunction*, float> repairFuncs;
    float destroyFitnessSum;
    float repairFitnessSum;
};

#endif
