#pragma once

#include <cmath>
#include <vector>

std::vector<double> percgrowth(std::vector<double> vec);

double mean_func(std::vector<double> vec);
double var(std::vector<double> vec);
double stdev_func(std::vector<double> vec);
double covar(std::vector<double> vec1, std::vector<double> vec2);

class Normal {
public:
	std::vector<double> vec;
	double mean;
	double stdev;
	Normal(std::vector<double> vec);
};

class CombNormal {
public:
	double mean;
	double stdev;
	CombNormal(std::vector<std::pair<Normal, double> >);
};