#ifndef DEFS_H
#define DEFS_H

enum Objective { MAXIMIZE, MINIMIZE };
enum Aggregate { MAX, MIN, SUM };
enum Measure { STAR, RADIUS, RAY };

struct ProblemType {
    Objective objective;
    Aggregate aggregate;
    Measure measure;
};

#endif