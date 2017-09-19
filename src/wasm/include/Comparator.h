#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "defs.h"

// a class to hide whether we are minimizing or maximizing the objective
// gets passed an Objective to make the distinction
class Comparator {
public:
    Comparator(Objective type=MINIMIZE) { this->type = type; }
    void setType(Objective type) { this->type = type; }
    Objective getType() { return this->type; }
    
    // this function returns true iff the first argument
    // is better than the second argument dependent on the objective type
    template <typename T>
    bool operator()(T left, T right) {
        switch (this->type) {
            case MINIMIZE:
                return left < right;
            case MAXIMIZE:
                return left > right;
            default:
                throw "Comparator: unrecognized type!";
        }
    }

    // returns the better of the two values dependent on the objective type
    // ties are resolved in favor of the second argument
    template <typename T>
    int getBetter(T left, T right) {
        switch (this->type) {
            case MINIMIZE:
                return (left < right ? left : right);
            case MAXIMIZE:
                return (left > right ? left : right);
        }
    }
private:
    Objective type;
};

#endif