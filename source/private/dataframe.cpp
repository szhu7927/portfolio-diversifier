#include "..\public\date.h"
#include "..\public\dataframe.h"

//Converts a date or an ETF to their index on the axis. Throws an exception if the index is not found. Case-sensitive, should stay in all caps.
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
	//The ETF should be in all caps.
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

void DataFrame::print() {
	std::cout << "{";
	for (int i = 0; i < etflength; ++i) {
		std::cout << "{ ";
		if (datelength > 3) {
			std::cout << DataFrame::getprice(0, i) << ", ";
			std::cout << DataFrame::getprice(1, i) << ", ";
			std::cout << "..., ";
			std::cout << DataFrame::getprice(datelength-1, i) << " ";
		}
		else {
			for (int j = 0; j < datelength; ++j) {
				std::cout << DataFrame::getprice(j, i) << ", ";
			}
		}
		
		if(i == etflength - 1) std::cout << "}}" << "\n";
		else std::cout << "}," << "\n";
	}

	std::cout << "Coordinates: " << "\n";
	std::cout << "ETF: (";
	
	if (etflength > 10) {
		for (int i = 0; i < 10; ++i) {
			std::cout << etfaxis[i] << ", ";
		}
		std::cout << etfaxis[etflength - 1];
	}
	else {
		for (int i = 0; i < etflength; ++i) {
			std::cout << etfaxis[i];
			if (i != etflength - 1) std::cout << ", ";
		}
	}

	std::cout << ")" << "\n";
	std::cout << "Dates: (";
	
	if (datelength > 10) {
		for (int i = 0; i < 10; ++i) {
			std::cout << dateaxis[i].tostring() << ", ";
		}
		std::cout << "..., " << dateaxis[datelength - 1].tostring();
	}
	else {
		for (int i = 0; i < datelength; ++i) {

			std::cout << dateaxis[i].tostring();
			if (i != datelength - 1) std::cout << ", ";
		}
	}

	std::cout << ")" << "\n";
}