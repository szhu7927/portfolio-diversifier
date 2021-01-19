#pragma once

#include <iostream>
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
Line getline(std::vector<std::pair<double, double>> frontier);

double intervalarea(std::vector<std::pair<double, double>> frontier, Line line, int start, int end);
double cumarea(std::vector<std::pair<double, double>> frontier, Line line, int point);
double totalarea(std::vector<std::pair<double, double>> frontier);
std::vector<std::pair<std::pair<double, double>, double>> cumareavector(std::vector<std::pair<double, double>> frontier);
std::pair<double, double> bestpoint(std::vector<std::pair<std::pair<double, double>, double>> areadata, double risk);

void printareas(std::vector<std::pair<std::pair<double, double>, double>> areadata);