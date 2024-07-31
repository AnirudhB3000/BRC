#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "methods/NaiveMethod.h"
#include "methods/MapReduce.h"
#include "utils/Result.h"
#include "utils/BillionRowChallenge.h"
#include <memory>

void compareMethods(
    const std::vector<std::unique_ptr<BillionRowChallenge>> &methods,
    std::map<std::string, double> &time_taken_map,
    std::map<std::string, std::vector<Result>> &results_map
    /*
    Args:
    methods: vector of all the classes to be compared
    time_taken_map: map of all the timings
    results: vector of the results.

    Functionality:
    Compares the methods specified in methods.
    */
    ) {
        for (const auto &method : methods) {
            double time_taken = method->process();
            std::vector<Result> results = method->getResults();

            std::string class_name = method->getClassName();

            time_taken_map[class_name] = time_taken;

            results_map[class_name] = results;

            std::cout << "\nProcessor function has been executed for " << class_name << "." << std::endl;

        }
    }

int main(int argc, char *argv[]) {
    /*
    Specify the input files if necessary. 
    If output file is specified, input needs to be specified (TBD: Fix)
    Output from main processing function is a vector of type Result.
    The processing function takes in two inputs; the file and the result vector.
    */
   
    std::string file = "data/weather_stations.csv";
    std::string fileToBeWrittenTo = "data/outputs/output";
    std::string timeTrackerFile = "data/tracker.txt";

    if (argc > 1) {
        file = argv[1];
    }

    if (argc > 2) {
        fileToBeWrittenTo = argv[2];
    }

    if (argc > 3) {
        timeTrackerFile = argv[3];
    }

    std::cout << "\nInput file " << file << " has been read." << std::endl;

    std::vector<std::unique_ptr<BillionRowChallenge>> methods;
    std::map<std::string, double> time_taken_map;
    std::map<std::string, std::vector<Result>> results_map;

    methods.push_back(std::make_unique<MapReduceMethod>(file));
    methods.push_back(std::make_unique<NaiveMethod>(file));

    compareMethods(methods, time_taken_map, results_map);


    for (auto iter : results_map) {
        std::string fileToBeWrittenTo_iter = fileToBeWrittenTo + "_" + iter.first + ".txt";
        
        std::ofstream outFile(fileToBeWrittenTo_iter);
        
        if (!outFile) {
            std::cerr << "\nError opening output file" << std::endl;
            return EXIT_FAILURE;
        }

        for (const auto& result : iter.second) {
            outFile << result.city << "=" << result.min << "/"
                    << result.sum / result.count << "/" << result.max 
                    << std::endl;
        }

        outFile.close();

        std::cout << "\nOutput file " << fileToBeWrittenTo_iter << " has been written to." << std::endl;

    }

    
    
    std::ofstream timeTrackerOut(timeTrackerFile);
    if (!timeTrackerOut) {
        std::cerr << "\nError opening time tracker file" << std::endl;
        return EXIT_FAILURE;
    }

    for (auto iter : time_taken_map) {
        timeTrackerOut << iter.first << ": " << iter.second << std::endl;
    }
    timeTrackerOut.close();
    std::cout << "\nTime tracker file " << timeTrackerFile << " has been written to." << std::endl;

    return 0;
}
