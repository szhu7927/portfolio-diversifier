#include "..\public\date.h"
#include "..\public\dataframe.h"

//Converts a date or an ETF to their index on the axis. Throws an exception if the index is not found.
int DataFrame::toindex(Date date) {
	for (int i = 0; i < datelength; ++i) {
		if (date.equals(dateaxis[i])) return i;
	}
	throw std::domain_error("Date not found in axis.");
}
int DataFrame::toindex(std::string etf) {
	for (int i = 0; i < etflength; ++i) {
		if (etf == etfaxis[i]) return i;
	}
	throw std::domain_error("ETF not found in axis.");
}

DataFrame::DataFrame(std::vector<Date> dateaxis, std::vector<std::string> etfaxis, std::vector<std::vector<double>> price) {
	//The dates make up the "x-axis" and the ETFs make up the "y-axis". Price[i][j] refer to the ith date and the jth ETF.
	datelength = dateaxis.size();
	etflength = etfaxis.size();
	this->dateaxis = dateaxis;
	this->etfaxis = etfaxis;
	this->price = price;
}

//Returns the price at a specific ETF for a specific date.
double DataFrame::getprice(int date, int etf) { return price[date][etf]; }
double DataFrame::getprice(Date date, int etf) { return price[toindex(date)][etf]; }
double DataFrame::getprice(int date, std::string etf) { return price[date][toindex(etf)]; }
double DataFrame::getprice(Date date, std::string etf) { return price[toindex(date)][toindex(etf)]; }

//Returns a vector of prices for all ETFs for a specific date.
std::vector<double> DataFrame::datedata(int date) { return price[date]; }
std::vector<double> DataFrame::datedata(Date date) { return price[toindex(date)]; }

//Returns a vector of prices for all dates for a specific ETF.
std::vector<double> DataFrame::etfdata(int etf) {
	std::vector<double> vectorbuilder;
	for (int i = 0; i < datelength; ++i) {
		vectorbuilder.push_back(price[i][etf]);
	}
	return vectorbuilder;
}
std::vector<double> DataFrame::etfdata(std::string etf) { return etfdata(toindex(etf)); }
