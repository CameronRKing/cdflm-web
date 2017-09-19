#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "NDPSO.h"
#include "Utils.h"
using namespace std;

int main() {
    // setup
    srand(time(NULL));
    Algorithm* ndpso;
    ndpso = new NDPSO();

    try {
        ProblemData data = Utils::getData("../problems/Daskin/city1990.grt");
        ProblemResults results = ndpso->optimize(data);
        cout << results.time << endl;
    } catch (const std::exception &e) {
        cout << e.what() << endl;
    }
    delete ndpso;
    return 0;
}
