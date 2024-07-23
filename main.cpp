#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <cstdlib>

struct Result {
    std::string city;
    int count;
    double sum, min, max;
};

int getCityIndex(const std::string &city, const std::vector<Result> &results) {
    for (size_t i = 0; i < results.size(); ++i) {
        if (results[i].city == city) {
            return i;
        }
    }
    return -1;
}

bool compareResults(const Result &a, const Result &b) {
    return a.city < b.city;
}

int main(int argc, char *argv[]) {
    std::string file = "input.txt";
    if (argc > 1) {
        file = argv[1];
    }

    std::ifstream fh(file);
    if (!fh) {
        std::cerr << "error opening file" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<Result> results;
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

    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << results[i].city << "=" << results[i].min << "/"
                  << results[i].sum / results[i].count << "/" << results[i].max
                  << (i < results.size() - 1 ? ", " : "");
    }
    std::cout << std::endl;

    return 0;
}
