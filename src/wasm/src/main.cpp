#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "NDPSO.h"
#include "ALNS.h"
#include "Utils.h"
#include "Logger.h"
using namespace std;

int main() {
    // setup
    srand(time(NULL));
    string db = "../storage/logs/database.sqlite";
    Logger* logger = new Logger(db);
    Algorithm* ndpso, *alns;
    ndpso = new NDPSO();
    ndpso->setLogger(logger);
    alns = new ALNS();
    alns->setLogger(logger);

    try {
        logger->logExperiment();
        // solve the Daskin problems
        ProblemData data = Utils::getData("../storage/testCases/Daskin/city1990.grt");
        Utils::optimizeForEachProblemType(ndpso, data);
        Utils::optimizeForEachProblemType(alns, data);

        // solve the OR-LIB problems
        // using ii now because I read that it's better practice. Getting in the habit
        for (int ii = 1; ii <= 40; ii++) {
            data = Utils::getData("../storage/testCases/ORLIB/pmed" + to_string(ii) + ".txt");
            Utils::optimizeForEachProblemType(ndpso, data);
            Utils::optimizeForEachProblemType(alns, data);
        }

        delete ndpso;
        delete alns;
        delete logger;
    } catch (const std::exception &e) {
        cout << e.what() << endl;
        delete ndpso;
        delete alns;
        delete logger;
    } catch (...) {
        delete ndpso;
        delete alns;
        delete logger;
    }
    return 0;
}
