#ifndef DEFS_H
#define DEFS_H

// star, radius, ray
// max, sum, min
enum ProblemType { MAX_STAR,   SUM_STAR,   MIN_STAR, 
                   MAX_RADIUS, SUM_RADIUS, MIN_RADIUS,
                   MAX_RAY,    SUM_RAY,    MIN_RAY};
                   
// each of the above problemtypes can be maximized or minimized
enum ObjectiveType { MAXIMIZE, MINIMIZE };

#endif