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
	bool equals(Date date);
};