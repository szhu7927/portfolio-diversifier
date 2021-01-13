#pragma once

#include <string>
#include <stdexcept>
#include <vector>

#include "ymath.h"
#include "dataframe.h"
#include "date.h"
#include "datapt.h"
#include "frontieralgo.h"

std::vector<DataPT> find_optimal_points(DataFrame df, std::vector<std::string> input_etf_list);