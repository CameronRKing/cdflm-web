#include "Utils.h"

#include <map>
#include <cmath>
#include <regex>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "defs.h"
#include "ProblemData.h"
#include "ProblemResults.h"
using namespace std;

/**
 * Splits a string into a vector of substrings based on on the delimiter
 *
 * @param string str
 * @param string delimiter
 * @return vector<string> tokens
 **/
vector<string> Utils::split(string str, string delimiter) {
    // turning the string into a vector
    // so I can get a mutable char* for strtok
    vector<char> vec(str.size()+1);
    memcpy( &vec.front(), str.c_str(), str.size() + 1 );
    char* cstr = vec.data();
    char* pch;
    pch = strtok(cstr,"/");
    vector<string> tokens;
    while (pch != NULL)
    {
        string tmp = pch;
        tokens.push_back(tmp);
        pch = strtok(NULL, "/");
    }
    return tokens;
}


/**
 * Returns a ProblemData instance by processing the appropriate file
 *
 * @param string filename
 * @return Problemdata data
 **/
ProblemData Utils::getData(string filename) {
    if (regex_match(filename, regex(".*/ORLIB/.*"))) {
        return parseORLIB(filename);
    } else if (regex_match(filename, regex(".*/Daskin/.*"))) {
        return parseDaskin(filename);
    } else {
        throw "Unfamiliar data path given!";
    }
}

/**
 * Parses a given ORLIB file into a ProblemData object
 *
 * @preconditions: assumes file exists, and that it follows the expected format
 *
 * @param string filename
 * @return ProblemData data
 **/
ProblemData Utils::parseORLIB(string filename) {
    ifstream infile;
    ProblemData data;
    int numEdges;

    // get the name of the file
    data.name = Utils::split(filename, "/").back();

    infile.open(filename);
    // read header at top of file: <num_nodes> <num_edges> <num_facilities (p-value, in other words)>
    infile >> data.numCustomers;
    infile >> numEdges;
    infile >> data.numFacilities;

    // build the cost/demand matrices
    // for now, all demand is 1
    for (int i = 0; i < data.numCustomers; i++) {
        vector<int> costs (data.numCustomers, 0);
        vector<int> demand (data.numCustomers, 1);
        data.costs.push_back(costs);
        data.demand.push_back(demand);
    }
    for (int i = 0; i < data.numCustomers; i++) {
        for (int j = 0; j < data.numCustomers; j++) {
            if (i == j) {
                data.costs[i][j] = 0;
            } else {
                data.costs[i][j] = 10000;
                data.costs[j][i] = 10000;
            }
        }
    }

    // read in the edges they give us
    int node1;
    int node2;
    int cost;
    while (!infile.eof()) {
        infile >> node1 >> node2 >> cost;
        data.costs[node1-1][node2-1] = cost;
        data.costs[node2-1][node1-1] = cost; 
    }

    // Floyd's Algorithm
    for (int i = 0; i < data.numCustomers; i++) {
        for (int j = 0; j < data.numCustomers; j++) {
            for (int k = 0; k < data.numCustomers; k++) {
                if (data.costs[i][k] + data.costs[k][j] < data.costs[i][j]) {
                    data.costs[i][j] = data.costs[i][k] + data.costs[k][j];
                }
            }
        }
    }

    infile.close();

    // default values
    data.type = MAX_STAR;
    data.objType = MINIMIZE;
    return data;
}

/**
 * Parses a given Daskin dataset
 *
 * @preconditions: assumes file exists, and that it follows the expected format
 *
 * @param string filename
 * @return ProblemData data
 **/
ProblemData Utils::parseDaskin(string filename) {
    ifstream infile;
    ProblemData data;

    // get the name of the file
    data.name = Utils::split(filename, "/").back();

    string tmp;
    float lng, lat;
    vector<vector<float>> nodeList;
    vector<float> coords;
    data.numCustomers = 0;
    // a line in this file contains these white-space separated fields (in order):
        // nodeNumber, lng, lat, demand1, demand2, fixedCost, cityName
    // for now, we only want lng and lat
    infile.open(filename);
    while (!infile.eof()) {
        infile >> tmp;
        if (tmp == "") break;

        data.numCustomers++;
        infile >> lng >> lat;
        coords.push_back(lng);
        coords.push_back(lat);
        nodeList.push_back(coords);
        coords.clear();
        // ignore the rest of the input
        getline(infile, tmp);
        tmp = "";
    }

    // build the cost/demand matrices
    // for now, all demand is 1
    for (int i = 0; i < data.numCustomers; i++) {
        vector<int> costs (data.numCustomers, 0);
        vector<int> demand (data.numCustomers, 1);
        data.costs.push_back(costs);
        data.demand.push_back(demand);
    }

    int cost;
    for (int i = 0; i < data.numCustomers; i++) {
        for (int j = 0; j < data.numCustomers; j++) {
            if (i == j) {
                data.costs[i][j] = 0;
            } else {
                cost = Utils::calcCost(nodeList[i], nodeList[j]);
                data.costs[i][j] = cost;
                data.costs[j][i] = cost;
            }
        }
    }

    infile.close();

    // default values
    data.type = MAX_STAR;
    data.objType = MINIMIZE;
    data.numFacilities = 5;
    return data;
}


/**
 * Calculates the distance between two sets of coordinates for use in Utils::parseDaskin()
 * Scales that distance up and truncates it so we get an integer number
 * Doesn't respect the actual calculation of lng/lat distances;
 * just uses the simple distance calculation from basic algebra
 *
 * @param coords1 <lng, lat>
 * @param coords2 <lng, lat>
 * @return int cost
 **/
int Utils::calcCost(const vector<float>& coords1, const vector<float>& coords2) {
    return (int)(100 * sqrt(pow(coords1[0] - coords2[0], 2.0) + pow(coords1[1] - coords2[1], 2.0)));
}

/**
 * Prints a white-space separated matrix
 * Prints based on the given size of the matrix, so it may overrun the terminal
 *
 * !!!!GOOD CANDIDATE FOR TEMPLATING AND SAVING INTO REUSABLE CODE SILO!!!!
 *
 * @param const vector<vector<int>>& matrix
 * @return void
 **/
void Utils::printMatrix(const vector<vector<int>>& matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

/**
 * Prints a vector, minimum 3 spaces per entry, 10 entries per line
 * 
 * !!!GOOD CANDIDATE FOR TEMPLATING AND SAVING INTO REUSABLE CODE SILO!!!
 *
 * @param const vector<int>& vect
 * @return void
 **/
void Utils::printVector(const vector<int>& vect) {
    for (int i = 1; i <= vect.size(); i++) {
        cout << setw(3) << vect[i-1];
        if (i % 10 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}


/**
 * Runs the given algorithm on the given data set for each problem type that we have
 * Logs results to the database
 *
 * @preconditions: assumes that MAX_STAR is the first ProblemType, and MIN_RAY the last
 *                 assumes that MINIMIZE is the last ObjectiveType, and MAXIMIZE the first
 *
 * @param Algorithm* alg
 * @param const ProblemData data
 * @return void
 **/
void Utils::optimizeForEachProblemType(Algorithm* alg, ProblemData data) {
    ProblemResults results;
    Listener* listener = alg->getListener();
    cout << alg->getName() << endl;
    data.objType = MINIMIZE;

    ProblemType types[4] = { SUM_STAR, MAX_RADIUS, MAX_STAR, SUM_RADIUS };
    for (int type = 0; type < 4; type++) {
        data.type = types[type];
        // run the problem ten times so we can take the best later
        for (int ii = 0; ii < 10; ii++) {
            listener->handleAlgorithm(alg, data.name, data.type, data.objType);
            results = alg->optimize(data);
            listener->handleResults(results);
        }
    }
}

