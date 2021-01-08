#pragma once

#include <string>
#include <stdexcept>
#include <vector>

class Date {

	int year;
	int month;
	int day;

public:
	Date(int month, int day, int year);
	Date(std::string datestring);
	bool equals(Date date);
	std::string tostring();
};