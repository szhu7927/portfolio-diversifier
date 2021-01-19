#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "date.h"

class DataFrame {
public:
	int datelength;
	int etflength;
	std::vector<Date> dateaxis;
	std::vector<std::string> etfaxis;
	std::vector<std::vector<double> > price;

	//Converts a date or an ETF to their index on the axis. Throws an exception if the index is not found.
	int toindex(Date date);
	int toindex(std::string etf);

	DataFrame(std::vector<Date> dateaxis, std::vector<std::string> etfaxis, std::vector<std::vector<double> > price);

	//Returns the price at a specific ETF for a specific date.
	double getprice(int date, int etf);
	double getprice(Date date, int etf);
	double getprice(int date, std::string etf);
	double getprice(Date date, std::string etf);

	//Returns a vector of prices for all ETFs for a specific date.
	std::vector<double> datedata(int date);
	std::vector<double> datedata(Date date);

	//Returns a vector of prices for all dates for a specific ETF.
	std::vector<double> etfdata(int etf);
	std::vector<double> etfdata(std::string etf);

	//Prints the dataframe
	void print();
};
