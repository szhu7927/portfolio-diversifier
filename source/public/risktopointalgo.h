#pragma once

#include <vector>
#include <algorithm>

double area(std::vector<std::pair<double, double>> vec);

class Line {
public:
	double slope;
	double intercept;
	Line(double slope, double intercept);
};

double gety(Line line, double x);
double totalarea(std::vector<std::pair<double, double>> frontier);