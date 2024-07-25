#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

struct Result {
    std::string city;
    int count;
    double sum, min, max;
};

class NaiveMethod {
    std::ifstream fh;
    std::vector<Result> results;

    int getCityIndex(const std::string &city, const std::vector<Result> &results) const {
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

    static bool compareResults(const Result &a, const Result &b) {
        /*
        Args:
        a, b: cities

        Functionality:
        Helper function for naiveMethod
        */
        return a.city < b.city;
    }

public:
    NaiveMethod(const std::string &file) {
        fh.open(file);
        if (!fh) {
            std::cerr << "\nError opening file" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    void process() {
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

    const std::vector<Result>& getResults() const {
        return results;
    }
};

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

    std::cout << "\nInput file " << fileToBeWrittenTo << " has been read." << std::endl;

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
