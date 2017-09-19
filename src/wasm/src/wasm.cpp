#include "../include/defs.h"
#include "../include/Algorithm.h"
#include "../include/ProblemData.h"
#include "../include/ProblemResults.h"
// Even though I never directly reference Particle,
// the EMSDK wants it explicitly bound or else it throws a fit during runtime
#include "../include/Particle.cpp"
#include "../include/Listener.h"
#include "../include/NDPSO.cpp"
#include "../include/Utils.cpp"
#include <vector>
#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;
using namespace std;

// basic wrapper to let me write Listeners in JS
// variadic functions don't play nice with virtualization
    // can't use basic variadic functions because I'm passing non-POD values
    // variadic templates can't be virtualized
// this method allows me to let the JavaScript object decide how/whether to handle different kinds of events
class ListenerWrapper : public wrapper<Listener> {
public:
    EMSCRIPTEN_WRAPPER(ListenerWrapper);
    void handleAlgorithm(Algorithm* alg, std::string name, ProblemType type) {
        return call<void>("handle", std::string("algorithm"), alg, name, type);
    }

    void handleResults(ProblemResults results) {
        return call<void>("handle", std::string("results"), results);
    }

    void handleParticle(Particle* particle, int iteration) {
        return call<void>("handle", std::string("particle"), particle, iteration);
    }
};

/* 

build command (until I get the EMSDK in the Docker container and thus can add an entry to the makefile; run from Default directory):

emcc --bind -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -O3 --std=c++11 --preload-file ../problems -o cdflm.js ../src/wasm.cpp

*/

ProblemData getORLIBData(int num) {
    return Utils::getData("../problems/ORLIB/pmed" + to_string(num) + ".txt");
}

ProblemData getDaskinData() {
    return Utils::getData("../problems/Daskin/city1990.grt");
}

EMSCRIPTEN_BINDINGS(cdflm_cpp) {
    register_vector<Particle>("VectorParticle");
    register_vector<int>("VectorInt");
    register_vector<vector<int>>("VectorVectorInt");

    enum_<Objective>("Objective")
        .value("MAXIMIZE", MAXIMIZE)
        .value("MINIMIZE", MINIMIZE);

    enum_<Aggregate>("Aggregate")
        .value("MAX", MAX)
        .value("MIN", MIN)
        .value("SUM", SUM);

    enum_<Measure>("Measure")
        .value("STAR", STAR)
        .value("RADIUS", RADIUS)
        .value("RAY", RAY);

    value_object<ProblemType>("ProblemType")
        .field("objective", &ProblemType::objective)
        .field("aggregate", &ProblemType::aggregate)
        .field("measure", &ProblemType::measure);

    value_object<ProblemData>("ProblemData")
        .field("name", &ProblemData::name)
        .field("type", &ProblemData::type)
        .field("numFacilities", &ProblemData::numFacilities)
        .field("numCustomers", &ProblemData::numCustomers)
        .field("costs", &ProblemData::costs)
        .field("demand", &ProblemData::demand);

    value_object<ProblemResults>("ProblemResults")
        .field("time", &ProblemResults::time)
        .field("objective", &ProblemResults::objective)
        .field("facilities", &ProblemResults::facilities)
        .field("customerAssignments", &ProblemResults::customerAssignments)
        .field("type", &ProblemResults::type);

    emscripten::function("getORLIBData", &getORLIBData);
    emscripten::function("getDaskinData", &getDaskinData);

    class_<Listener>("Listener")
        .function("handleAlgorithm", &Listener::handleAlgorithm, pure_virtual(), allow_raw_pointers())
        .function("handleResults", &Listener::handleResults, pure_virtual())
        .function("handleParticle", &Listener::handleParticle, pure_virtual(), allow_raw_pointers())
        .allow_subclass<ListenerWrapper>("ListenerWrapper");

    class_<Algorithm>("Algorithm")
        .function("setListener", &Algorithm::setListener, allow_raw_pointers());

    class_<Particle>("Particle");

    class_<NDPSO, base<Algorithm>>("NDPSO")
        .constructor<>()
        .function("optimize", &NDPSO::optimize)
        .function("getName", &NDPSO::getName)
        .function("getJSONParameters", &NDPSO::getJSONParameters);
}