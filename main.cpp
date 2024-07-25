#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <cstdlib>

//TODO: Make getCityIndex and compareResults private functions in a NaiveMethod class

struct Result {
    std::string city;
    int count;
    double sum, min, max;
};

int getCityIndex(const std::string &city, const std::vector<Result> &results) {
    /*
    Args:
    city: string of city
    results: required data

    Functionality:
    Helper function for naiveMethod, returns the index of the city.
    */

    for (size_t i = 0; i < results.size(); ++i) {
        if (results[i].city == city) {
            return i;
        }
    }
    return -1;
}

bool compareResults(const Result &a, const Result &b) {
    /*
    Args:
    a, b: cities

    Functionality:
    Helper function for naiveMethod
    */
    return a.city < b.city;
}

void naiveMethod(std::ifstream &fh, std::vector<Result> &results) {
    /*
    Args:
    fh: the file scanned from disk
    results: the output vector
    
    Functionality:
    Modified the vector results to the required output
    */
    std::string line;

    while (std::getline(fh, line)) {
            size_t pos = line.find(';');
            if (pos == std::string::npos) continue;

            std::string city = line.substr(0, pos);
            double measurement = std::stod(line.substr(pos + 1));

            int c = getCityIndex(city, results);
            if (c < 0) {
                Result res = { city, 1, measurement, measurement, measurement };
                results.push_back(res);
            } else {
                results[c].sum += measurement;
                results[c].count += 1;
                if (results[c].min > measurement) {
                    results[c].min = measurement;
                }
                if (results[c].max < measurement) {
                    results[c].max = measurement;
                }
            }
        }

    std::sort(results.begin(), results.end(), compareResults);
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
    if (argc > 1) {
        file = argv[1];
    }

    if (argc > 2) {
        file = argv[1];
        fileToBeWrittenTo = argv[2];
    }

    std::ifstream fh(file);
    if (!fh) {
        std::cerr << "\nerror opening file" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Input file " << file << " has been processed...";

    std::vector<Result> results;

    naiveMethod(fh, results);

    std::cout << "\nProcessing function executed...";
    
    std::ofstream outFile(fileToBeWrittenTo);
    if (!outFile) {
        std::cerr << "\nerror opening output file" << std::endl;
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < results.size(); ++i) {
        outFile << results[i].city << "=" << results[i].min << "/"
                << results[i].sum / results[i].count << "/" << results[i].max 
                << std::endl;
    }

    std::cout << "\nOutput file " << fileToBeWrittenTo << " has been written to.";

    return 0;
}
