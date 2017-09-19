#ifndef LISTENER_H
#define LISTENER_H 

#include "Algorithm.h"
#include "Particle.h"
#include "ProblemResults.h"
#include "defs.h"
#include <string>

class Algorithm;

class Listener {
public:
    virtual void handleAlgorithm(Algorithm*, std::string, ProblemType) = 0;
    virtual void handleResults(ProblemResults) = 0;
    virtual void handleParticle(Particle*, int) = 0;
};

#endif