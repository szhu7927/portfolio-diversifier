#pragma once

#include <string>
#include <stdexcept>
#include <vector>

#include "ymath.h"
#include "dataframe.h"
#include "date.h"
#include "frontieralgo.h"

std::vector<CombNormal> standard_algo(std::vector<CombNormal> rawlist);

std::vector<std::vector<double>> weight_generator(double inc, int length);

//Precondition: inc must evenly divide 1.
std::vector<CombNormal> raw_point_generator(DataFrame df, std::vector<std::string> input_etf_list, double inc);

//Precondition: inc must evenly divide 1.
std::vector<CombNormal> find_optimal_points(DataFrame df, std::vector<std::string> input_etf_list, double increment);

void fa_debug();