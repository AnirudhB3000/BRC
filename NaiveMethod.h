#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "Result.h"
#include <chrono>
#include "BillionRowChallenge.h"

class NaiveMethod : public BillionRowChallenge {
    std::ifstream fh;
    std::vector<Result> results;

    int getCityIndex(const std::string &city, const std::vector<Result> &results) const {
        /*
        Args:
        city: string of city
        results: required data

        Functionality:
        Helper function for process(), returns the index of the city.
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
        Helper function for process()
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

    double process() override {
        /*
        Functionality:
        Modified the vector results to the required output
        Returns the time of execution
        */
        auto start = std::chrono::high_resolution_clock::now();

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

        fh.close();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        return duration.count();
    }

    const std::vector<Result>& getResults() const override {
        return results;
    }

    std::string getClassName() const override {
        return "Naive Method";
    }
};