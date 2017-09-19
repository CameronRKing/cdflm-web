#ifndef NDPSO_H
#define NDPSO_H

#include "Algorithm.h"
#include "Particle.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// default values for parameters
const float SOCIAL    = 0.7;
const float COGNITIVE = 0.7;
const float INERTIA   = 0.7;
const float INERTIAL_DISCOUNT = 0.9995;
const int   SWARM_SIZE     = 100;
const int   MAX_ITERATIONS = 1000;



class NDPSO : public Algorithm {
friend class Particle;
public:
    NDPSO();                                      // use default parameters defined as constants in this file
    NDPSO(int);                                   // just for setting maxIterations
    NDPSO(float, float, float, float, int, int);  // set all parameters
    ~NDPSO() {};
    ProblemResults optimize(ProblemData) override;
    string getName() override { return "NDPSO"; };
    string getJSONParameters() override;
    void setInertia(float c1) { this->inertia = c1; this->initialInertia = c1; }
    void setSocial(float c2) { this->social = c2; }
    void setCognitive(float c3) { this->cognitive = c3; }

    /* overridden functions */
        // Algorithm::calcObjective() && friends expects a vector of CUSTOMER ASSIGNMENTS
        // however, this metaheuristic works by manipulating a vector of FACILITY ASSIGNMENTS
        // we don't track or calculate customer assignments, so we need our own objective functions
    int calcObjective(const vector<int>&) override;
private:
    /* members */
    vector<Particle> swarm;
    int   swarmSize;
    int   maxIterations;
    float cognitive;
    float social;
    float inertia;          // since inertia is discounted, but we don't want the user to have to worry about it,
    float initialInertia;   // we'll save the given inertia into initialInertia and reset inertia to initialInertia in optimize()
    float inertialDiscount;

    /* functions */
    void initSwarm();
    Particle getGlobalBest();
    Particle getGlobalWorst(); 
};

#endif