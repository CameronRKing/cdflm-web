#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include <string>
// #include "NDPSO.h"
using namespace std;

// forward declaration because otherwise the compiler is NOT happy. At all. 
class NDPSO;

class Particle {
public:
    /* functions */
    Particle(int, int, NDPSO*);
    void update(const Particle&);
    vector<int> getCustomerAssignments();
    string getJSONFacilities();
    string getJSONCustomers();
    vector<int> position;
            int fitness;
    vector<int> pBestPosition;
            int pBestFitness;

private:
    void updateSinglePosition(vector<int>&, int&, const float);
    vector<int> exchange(const vector<int>&);

    /* members */
         NDPSO* ndpso;          // since nested classes don't work QUITE like I'd hoped, we need to save a reference to the enclosing NDPSO object
};

#endif