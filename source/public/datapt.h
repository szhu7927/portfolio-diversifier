#pragma once

#include <string>
#include <stdexcept>
#include <vector>

#include "ymath.h"
#include "dataframe.h"
#include "date.h"

class DataPT {
public:
	double reward; //Expected return value, the mean growth
	double risk; //Standard deviation of growth
	std::vector<std::pair<std::string, double>> ETF_weights; //A list of all used ETFs, and their corresponding weights.

	DataPT(double reward, double risk, std::vector<std::pair<std::string, double>> ETF_weights);
};