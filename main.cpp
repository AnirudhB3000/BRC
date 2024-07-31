#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "NaiveMethod.h"
#include "Result.h"
#include "MapReduce.h"
#include "BillionRowChallenge.h"
#include <memory>

void compareMethods(
    const std::vector<std::unique_ptr<BillionRowChallenge>> &methods,
    std::map<std::string, double> &time_taken_map,
    std::vector<Result> &results
    ) {
        for (const auto &method : methods) {
            double time_taken = method->process();
            const std::vector<Result> &results = method->getResults();

            std::string class_name = method->getClassName();

            std::cout << "\nProcessor function has been executed for " << class_name << "." << std::endl;

            time_taken_map[class_name] = time_taken;
        }
    }

int main(int argc, char *argv[]) {
    /*
    Specify the input files if necessary. 
    If output file is specified, input needs to be specified (TBD: Fix)
    Output from main processing function is a vector of type Result.
    The processing function takes in two inputs; the file and the result vector.
    */
   
    std::string file = "weather_stations.csv";
    std::string fileToBeWrittenTo = "output.txt";
    std::string timeTrackerFile = "tracker.txt";

    if (argc > 1) {
        file = argv[1];
    }

    if (argc > 2) {
        fileToBeWrittenTo = argv[2];
    }

    std::cout << "\nInput file " << file << " has been read." << std::endl;


    // MapReduceMethod brc(file);
    // double time_taken = brc.process();
    // const std::vector<Result>& results = brc.getResults();

    // std::cout << "\nProcessor function has been executed." << std::endl;

    // std::map<std::string, double> time_taken_map;
    // time_taken_map["Map Reduce Method"] = time_taken;

    // NaiveMethod brc2(file);
    // double time_taken2 = brc2.process();
    // const std::vector<Result>& results2 = brc.getResults();
    // time_taken_map["Naive Method"] = time_taken2;
    std::vector<std::unique_ptr<BillionRowChallenge>> methods;
    std::map<std::string, double> time_taken_map;
    std::vector<Result> results;

    methods.push_back(std::make_unique<MapReduceMethod>(file));
    methods.push_back(std::make_unique<NaiveMethod>(file));

    compareMethods(methods, time_taken_map, results);

    std::ofstream outFile(fileToBeWrittenTo);
    if (!outFile) {
        std::cerr << "\nError opening output file" << std::endl;
        return EXIT_FAILURE;
    }

    for (const auto& result : results) {
        outFile << result.city << "=" << result.min << "/"
                << result.sum / result.count << "/" << result.max 
                << std::endl;
    }

    outFile.close();
    std::cout << "\nOutput file " << fileToBeWrittenTo << " has been written to." << std::endl;

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
