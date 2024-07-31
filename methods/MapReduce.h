#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <map>
#include <chrono>
#include "../utils/Result.h"
#include "../utils/BillionRowChallenge.h"

class MapReduceMethod : public BillionRowChallenge{
    std::ifstream fh;
    std::vector<Result> results;

    std::map<std::string, std::vector<double>> mapPhase() {
        std::map<std::string, std::vector<double>> mappedData;
        std::string line;
        while (std::getline(fh, line)) {
            size_t pos = line.find(';');
            if (pos == std::string::npos) continue;

            std::string city = line.substr(0, pos);
            double measurement = std::stod(line.substr(pos + 1));
            mappedData[city].push_back(measurement);
        }
        return mappedData;
    }

    std::vector<Result> reducePhase(const std::map<std::string, std::vector<double>>& mappedData) {
        std::vector<Result> reducedData;
        for (const auto& pair : mappedData) {
            const std::string& city = pair.first;
            const std::vector<double>& measurements = pair.second;

            if (measurements.empty()) continue;

            Result res;
            res.city = city;
            res.count = measurements.size();
            res.sum = std::accumulate(measurements.begin(), measurements.end(), 0.0);
            res.min = *std::min_element(measurements.begin(), measurements.end());
            res.max = *std::max_element(measurements.begin(), measurements.end());
            reducedData.push_back(res);
        }
        return reducedData;
    }

    static bool compareResults(const Result &a, const Result &b) {
        /*
        Args:
        a, b: cities

        Functionality:
        Helper function for process()
        */
        return a.city < b.city;
    }

public:
    MapReduceMethod(const std::string &file) {
        fh.open(file);
        if (!fh) {
            std::cerr << "\nError opening file" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    double process() override {
        /*
        Functionality:
        Modified the vector results to the required output
        Returns the time of execution
        */
        auto start = std::chrono::high_resolution_clock::now();
        

        auto mappedData = mapPhase();
        results = reducePhase(mappedData);
        std::sort(results.begin(), results.end(), compareResults);


        fh.close();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        return duration.count();
    }

    const std::vector<Result>& getResults() const override {
        return results;
    }

    std::string getClassName() const override {
        return "MapReduceMethod";
    }
};