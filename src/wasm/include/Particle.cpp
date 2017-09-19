#include "Particle.h"
#include "NDPSO.h"
#include "Utils.h"

#include <map>
#include <limits>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

string Particle::getJSONFacilities() {
    string json = "[";
    for (int fac : position) { json += to_string(fac) + ", "; }
    return json + "]";
}

string Particle::getJSONCustomers() {
    vector<int> custs = getCustomerAssignments();
    string json = "[";
    for (int cust : custs) { json += to_string(cust) + ", "; }
    return json + "]";
}

/**
 * Updates the position of a particle
 * This algorithm uses a different PSO method: we perform random one-facility-exchanges on different facility vectors with the probability of different parameters
 *     + current position is governed by inertia
 *     + persaonl best is governed by the cognitive parameter
 *     + global best is governed by the social parameter
 * After all exchanges have (potentially) been performed, we take the best of the three.
 * Note that this new position vector's objective value does NOT have to be better than the Particle's current position vector's value!
 *
 * @preconditions: assumes the particle has been initialized (pBestPosition && pBestFitness have been set)
 * @postconditions: promises to update this particle's current position and fitness, as well as its personal best position/fitness
 * @param const Particle& gBest (global best Particle)
 **/
void Particle::update(const Particle& gBest) {
    vector<int> s1 = position,
                s2 = pBestPosition,
                s3 = gBest.position;
    int s1Fitness = fitness,
        s2Fitness = pBestFitness,
        s3Fitness = gBest.fitness;

    this->updateSinglePosition(s1, s1Fitness, ndpso->inertia);
    this->updateSinglePosition(s2, s2Fitness, ndpso->cognitive);
    this->updateSinglePosition(s3, s3Fitness, ndpso->social);

    // get best of the new exchanges
    int best;
    best = ndpso->comparator.getBetter(s1Fitness, s2Fitness);
    best = ndpso->comparator.getBetter(s3Fitness, best);
    if (best == s1Fitness) {
        this->position = s1;
    } else if (best == s2Fitness) {
        this->position = s2;
    } else {
        this->position = s3;
    }
    this->fitness = best;

    // update personal best, if needed
    if (ndpso->comparator(this->fitness, this->pBestFitness)) {
        this->pBestPosition = this->position;
        this->pBestFitness  = this->fitness;
    }
}

/**
 * Updates a single position. Modifies the first two parameters in place.
 *
 * @param vector<int>& position
 * @param int& fitness
 * @param float probability
 * @param bool shouldExclude=false
 * @return void
 **/
void Particle::updateSinglePosition(vector<int>& position, int& fitness, const float probability) {
    if (rand() % 100 / 100.0 <= probability) {
        position = exchange(position);
        fitness  = ndpso->calcObjective(position);
    }
}

/**
 * Exchanges one facility in a Particle's position vector
 * Returns a NEW vector--does NOT change the input parameter!
 *
 * @param const vector<int>& position
 * @return vector<int> new position vector with ONE facility exchanged
 **/
vector<int> Particle::exchange(const vector<int>& pos) {
    int possibleFacs = ndpso->data.numCustomers;
    int toUnassign;
    int newFacility;
    vector<int> toReturn (pos);

    toUnassign = rand() % pos.size();
    do {
        newFacility = rand() % possibleFacs;
    } while (find(pos.begin(), pos.end(), newFacility) != pos.end());

    toReturn[toUnassign] = newFacility;
    return toReturn;
}

/**
 * Calculates and returns the customer/facility assignments for this particle
 *
 * @preconditions: assumes that facilities have been assigned in this particle
 * @postconditions: promises to change nothing in the particle, but returns customer assignments based on a heuristic
 * @return: vector of customer assignements. Index represents customer number, the value represents the facility
 **/
vector<int> Particle::getCustomerAssignments() {
    return (ndpso->data).assignCustomers(this->position);
}

/**
 * Constructor for the Particle structure
 * Initializes random facility assignments
 *
 * @preconditions: assumes the problem has been initialized, and that srand() has been seeded
 * @postconditions: promises to be a fully-functional, ready-to-rock Particle when done
 * @param int numDimensions: in this case, the number of facilities to open
 * @param int possibleFacs: the exclusive upper bound of the numbers denoting which facilities can be opened
 *     --e.g., if we can open facilities in nodes [0, 99], then possibleFacs = 100
 * @param NDPSO* ndpso: a reference to the enclosing NDPSO instance (so we can access the data and parameters later)
 **/
Particle::Particle(int numDimensions, int possibleFacs, NDPSO* ndpso) {
    this->ndpso = ndpso;

    int fac;
    vector<int>::iterator it;

    // randomly assign facilities in the position vector
    for (int i = 0; i < numDimensions; i++) {
        // assign a facility that hasn't yet been assigned
        do {
            fac = rand() % possibleFacs;
            it = find(position.begin(), position.end(), fac);
        } while (it != position.end());
        position.push_back(fac);
    }

    // calculate fitness and assign personal bests
    fitness = ndpso->calcObjective(position);
    pBestPosition = position;
    pBestFitness  = fitness;
}
