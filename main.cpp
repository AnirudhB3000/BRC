#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "NaiveMethod.h"
#include "Result.h"

int main(int argc, char *argv[]) {
    /*
    Specify the input files if necessary. 
    If output file is specified, input needs to be specified (TBD: Fix)
    Output from main processing function is a vector of type Result.
    The processing function takes in two inputs; the file and the result vector.
    */
   
    std::string file = "weather_stations.csv";
    std::string fileToBeWrittenTo = "output.txt";
    if (argc > 1) {
        file = argv[1];
    }

    if (argc > 2) {
        fileToBeWrittenTo = argv[2];
    }

    std::cout << "\nInput file " << file << " has been read." << std::endl;

    NaiveMethod nm(file);
    nm.process();
    const std::vector<Result>& results = nm.getResults();

    std::cout << "\nProcessor function has been executed." << std::endl;

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

    std::cout << "\nOutput file " << fileToBeWrittenTo << " has been written to." << std::endl;

    return 0;
}
