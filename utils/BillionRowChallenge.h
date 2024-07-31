#pragma once
#include <vector>
#include "Result.h"

class BillionRowChallenge {
public:
    virtual ~BillionRowChallenge() = default;
    virtual double process() = 0;
    virtual const std::vector<Result>& getResults() const = 0;
    virtual std::string getClassName() const = 0;
};