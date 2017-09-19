#include "ALNS.h"

#include <map>
#include <cmath>
#include <vector>
#include <cstdlib>
#include "NDPSO.h"
#include "ALNSSolution.h"
#include "alns-functions.h"
using namespace std;



/**
 * Default constructor
 * Initializes parameters to constants found in ALNS.h
 * How do we handle dependency injection of the repair/destroy functions?
 * 
 * @preconditions: none
 * @postconditions: promises that all parameters EXCEPT temperature will be set
 *                      because temperature requires an initial solution to calculate,
 *                      we'll calculate it at the start of ALNS::optimize()
 **/
ALNS::ALNS() {
    this->maxIterations  = ALNS_MAX_ITERS;
    this->segmentLength  = SEGMENT_LENGTH;
    this->reactionFactor = REACTION_FACTOR;
    this->coolingFactor  = COOLING_FACTOR;
    this->startTempCtrl  = START_TEMP_CTRL;
    initDefaultFuncs();
    /** 
     * key for OUTCOME_SCORES:
     *     0: new solution was not accepted.
     *     1: new solution was the new global best
     *     2: new solution is better than current solution, and has not been accepted before
     *     3: new solution is worse than current solution, but has not been accepted before
     */
}

/**
 * Destructor to clean out dynamically allocated memory in destroyFuncs and repairFuncs
 * 
 * @postconditions: promises to delete all repair and destroy functions.
 *                  Does NOT delete the Listener! That is the responsibility of the main program!
 **/
ALNS::~ALNS() {
    for (auto &pair : this->destroyFuncs) {
        delete pair.first;
    }
    for (auto &pair : this->repairFuncs) {
        delete pair.first;
    }
}

string ALNS::getJSONParameters() {
    string json = "{";
    json += "maxIterations: "       + to_string(this->maxIterations);
    json += ", segmentLength: "        + to_string(this->segmentLength);
    json += ", reactionFactor: "           + to_string(this->reactionFactor);
    json += ", coolingFactor: " + to_string(this->coolingFactor);
    json += ", startTempCtrl: "        + to_string(this->startTempCtrl);
    json += ", newBestReward: "    + to_string(this->getNewBestReward());
    json += ", acceptedBetterReward: "    + to_string(this->getAcceptedBetterReward());
    json += ", acceptedWorseReward: "    + to_string(this->getAcceptedWorseReward());
    json += "}";
    return json;
}

/**
 * Initializes destroyFuncs && repairFuncs to default settings
 * Also sets repairFitnessSum and destroyFitnessSum
 **/
void ALNS::initDefaultFuncs() {
    // destroy functions
    this->destroyFuncs[new FacRandQDestroy(1)]  = 1.0;
    this->destroyFuncs[new FacBestQDestroy(1)]  = 1.0;
    this->destroyFuncs[new FacWorstQDestroy(1)] = 1.0;
    this->destroyFitnessSum = 3.0;

    // repair functions
    this->repairFuncs[new FacRandRepair()] = 1.0;
    this->repairFuncs[new FacLSRepair()]   = 1.0;
    this->repairFitnessSum = 2.0;
}

void ALNS::resetFuncFitnesses() {
    this->destroyFitnessSum = 0.0;
    for (auto& func : this->destroyFuncs) {
        this->destroyFitnessSum += 1.0;
        func.second = 1.0;
    }

    this->repairFitnessSum = 0.0;
    for (auto& func : this->repairFuncs) {
        this->repairFitnessSum += 1.0;
        func.second = 1.0;
    }
}

/**
 * Generates an initial solution at random for our main optimizing loop to work with
 * Currently generates the solution by running a very, very short NDPSO
 *
 * @preconditions: assumes this.data has been set
 * @postconditions: promises not to change any members
 * @return ALNSSolution --> a feasible initial solution
 **/
ALNSSolution ALNS::generateInitialSolution() {
    NDPSO* ndpso = new NDPSO(10);
    ProblemResults results = ndpso->optimize(this->data);
    delete ndpso;
    ALNSSolution solution { this->data, results.objective, results.facilities, results.customerAssignments, 0};
    return solution;

}

/**
 * Calculates the starting temperature for our simulated annealing acceptance test (see ALNS::accept())
 * The start temperature is set such that a solution that is w percent worse than 
 * the current solution is accepted with probability 0.5. (w is startTempCtrl)
 * 
 * @preconditions: 1) assumes the solution given is feasible
 *                 2) assumes that w (start temperature control parameter) has been set
 * @postconditions: sets the value of the temperature parameter
 * @param: ALNSSolution initial --> initial problem solution
 **/
void ALNS::calcStartingTemp(ALNSSolution initial) {
    // hopefully my algebra is correct
    this->temperature = initial.objective * (1.0 - startTempCtrl) / log(0.5);
}

/**
 * Performs a grid search for a parameter using a given setter
 * Runs each parameter set five times on this->data and saves the average objective
 * Takes the best average and sets it as the value of the parameter
 *
 * @preconditions: assumes that all parameters and this->data have been set
 * @postconditions: promises the parameter will have the best value we've found when done
 * 
 * @param float lower
 * @param float upper
 * @param float step
 * @param void (ALNS::*setter)(float) --> really messy syntax to declare a pointer to a member function
 * @return void
 **/
void ALNS::gridSearch(float lower, float upper, float step, void (ALNS::*setter)(float)) {
    map<float, float> avgObjectives;
    int sumObjectives;

    for (float param = lower; param <= upper; param += step) {
        sumObjectives = 0;
        (this->*setter)(param); // need the parentheses around this->*setter or the compiler throws a fit
        cout << "param: " << param << endl;
        for (int i = 0; i < 5; i++) {
            cout << "    running " << i << endl;
            this->listener->handleAlgorithm(this, this->data.name, this->data.type, this->data.objType);
            ProblemResults results = this->optimize(this->data);
            this->listener->handleResults(results);
            sumObjectives += results.objective;
        }
        avgObjectives[param] = sumObjectives / 5.0;
        cout << "  Avg: " << sumObjectives / 5.0 << endl;
    }

    // the parameter value associated with that average is now the official value
    Comparator comparator(this->data.objType);
    pair<float, float> best = *min_element(avgObjectives.begin(), avgObjectives.end(),
                                [&](pair<float, float> a, pair<float, float>b) {
                                    return comparator(a.second, b.second);
                                });
    cout << "FOUND: " << best.first << endl;
    (this->*setter)(best.first);
}

/**
 * Optimizes a given problem using Adaptive Large Neighborhood Search
 * generates a starter solution at random, then destroys/repairs the solution in various ways
 * until we reach max number of iterations
 *
 * @preconditions: assumes parameters have been initialized
 * @postconditions: promises to attempt an optimal solution
 * @param ProblemData data --> the problem to solve (MAX_STAR or SUM_RADIUS)
 * @return ProblemResults --> an attempt at an optimal solution
 **/
ProblemResults ALNS::optimize(ProblemData data) {
    clock_t begin = clock();
    this->data = data;
    this->comparator.setType(data.objType);
    int outcome;
    float score;
    FuncPair funcs;
    ALNSFunction* repair;
    ALNSFunction* destroy;
    ALNSSolution currentSolution, bestSolution, newSolution;
    
    // setup
    resetFuncFitnesses();
    currentSolution = generateInitialSolution();
    bestSolution    = currentSolution;
    calcStartingTemp(currentSolution);

    for (int count = 1; count <= this->maxIterations; count++) {
        funcs   = selectFuncs();
        repair  = funcs.repair;
        destroy = funcs.destroy;
        newSolution = (*destroy)(currentSolution);
        newSolution = (*repair)(newSolution);

        if (accept(newSolution, currentSolution)) {
            if (this->comparator(newSolution.objective, currentSolution.objective)) {
                outcome = 2;
            } else {
                outcome = 3;
            }
            currentSolution = newSolution;

            // if the solution is not accepted, there is no need to update the bestSolution,
            // so this block is fine inside this if statement
            if (this->comparator(currentSolution.objective, bestSolution.objective)) {
                bestSolution = currentSolution;
                outcome = 1;
            }
        } else {
            outcome = 0;
        }


        // update temperature
        this->temperature *= this->coolingFactor;

        // update function scores for this segment
        score = this->outcomeScores[outcome];
        repair->addToScore(score);
        destroy->addToScore(score);

        // update function fitnesses if we've hit the end of a segment
        if (count % this->segmentLength == 0) {
            updateFuncFitnesses();
        }
    }
    ProblemResults results {
                               float(clock() - begin) / CLOCKS_PER_SEC,
                               bestSolution.objective,
                               bestSolution.facilities,
                               bestSolution.customerAssignments,
                               data.type,
                               data.objType
                           }; 
    return results;
}

/**
 * Selects a set of destroy/repair functions using fitness (roulette wheel) selection
 * 
 * @preconditions:
 * @postcondition:
 * @return FuncPair --> struct containing pointers to repair/destroy functional objects
 **/
FuncPair ALNS::selectFuncs() {
    FuncPair funcs;

    // choose a destroy function
    float runningSum = 0.0;
    float randNum = (rand() % 100 + 1) / 100.0 * destroyFitnessSum;
    for (auto pair : destroyFuncs) {
        runningSum += pair.second;
        if (runningSum >= randNum) {
            funcs.destroy = pair.first;
            break;
        }
    }

    // choose a repair function
    randNum = (rand() % 100 + 1) / 100.0 * repairFitnessSum;
    runningSum = 0.0;
    for (auto pair : repairFuncs) {
        runningSum += pair.second;
        if (runningSum >= randNum) {
            funcs.repair = pair.first;
            break;
        }
    }

    return funcs;
}

/**
 * Determines whether to accept a new solution
 * Two criteria:
 *     1) Has the solution been visited before? --> checks a hash table of solutions
 *     2) Does the solution pass a simulated annealing acceptance test?
 *         -- accepted with probability e ^ (-(fitness(newSolution) - fitness(currentSolution)) / Temperature)
 * @preconditions: assumes that starting temperature has been calculated and set
 *                 assumes that both solutions are feasible
 *                 assumes that the hash table of visited solutions has been initialized (i.e., is not null)
 * @postconditions: promises that if the new solution is accepted, it will be hashed and added to the visited solutions
 * @param ProblemResults newSolution: the destroyed/repaired version of oldSolution
 * @param ProblemResults oldSolution: the solution from the previous iteration
 * @return bool --> whether the new solution is acceptable
 **/
bool ALNS::accept(ALNSSolution& newSolution, ALNSSolution& currentSolution) {
    bool shouldAccept = false;
    string hash = newSolution.getHash();
    if (visited.count(hash) > 0) {
        return false;
    }

    float randNum = (rand() % 100) / 100.0;
    float acceptanceChance = exp((currentSolution.objective - newSolution.objective) / this->temperature);
    if (randNum <= acceptanceChance) {
        shouldAccept = true;
        visited[hash] = true;
    }

    return shouldAccept;
}

/**
 * Updates the fitness of our facility functions based on their scores over the last segment
 * At the end of a segment (NOT an iteration!), update the "fitness" of each function using this formula:
 *     p<new> = p<old>(1 - r) + r(score<heuristic> / number of times we used the heuristic in the last segment)
 * 
 * @preconditions: assumes we've run a whole segment, and that each heuristic has been used at least once
 * @postconditions: 1) updates the MAP of function fitnesses
 *                  2) updates the SUMS of function fitnesses
 **/    
void ALNS::updateFuncFitnesses() {
    // destroy functions
    destroyFitnessSum = 0.0;
    for (auto &pair : destroyFuncs) {
        if (pair.first->getTimesUsed() > 0) {
            pair.second = pair.second * (1.0 - reactionFactor) + 
                            reactionFactor * (pair.first->getScore() / pair.first->getTimesUsed());
        }
        pair.first->reset();
        destroyFitnessSum += pair.second;
    }

    // repair functions
    repairFitnessSum = 0.0;
    for (auto &pair : repairFuncs) {
        if (pair.first->getTimesUsed() > 0) {
            pair.second = pair.second * (1.0 - reactionFactor) + 
                            reactionFactor * (pair.first->getScore() / pair.first->getTimesUsed());
        }
        pair.first->reset();
        repairFitnessSum += pair.second;
    }
}