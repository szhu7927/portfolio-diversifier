#pragma once

#include <string>
#include <stdexcept>
#include <vector>

class Date {
public:
	int year;
	int month;
	int day;

	//Standard constructor
	Date(int year, int month, int day);

	//Takes a string of the form 2021-07-19 and makes it into a Date obbject.
	Date(std::string datestring);

	//See if two Date objects represent the same date.
	bool equals(Date date);

	//Takes a Date object and returns a strong of the form 2021-07-19.
	std::string tostring();
};