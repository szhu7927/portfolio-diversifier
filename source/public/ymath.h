#pragma once

#include <cmath>
#include <string>
#include <iostream>
#include <vector>

std::vector<double> percgrowth(std::vector<double> vec);

double mean_func(std::vector<double> vec);
double var(std::vector<double> vec);
double stdev_func(std::vector<double> vec);
double covar(std::vector<double> vec1, std::vector<double> vec2);

class Normal {
public:
	std::string etf;
	std::vector<double> vec;
	double mean;
	double stdev;
	Normal(std::string etf, std::vector<double> vec);
};

class CombNormal {
	//AKA a DataPoint
public:
	double mean;
	double stdev;
	std::vector<std::pair<std::string, double>> ETF_weights; //A list of all used ETFs, and their corresponding weights.
	CombNormal(std::vector<std::pair<Normal, double>>);
	CombNormal(double mean, double stdev); //Direct implementation for debugging
	void print();
	void xyprint();
};
