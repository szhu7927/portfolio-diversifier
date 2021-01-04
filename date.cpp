#include "date.h"

Date::Date(int month, int day, int year) {
	//assertion
	if(month < 1 || month > 12) throw std::invalid_argument("Month must be an integer from 1 to 12.");
	if(day < 1 || day > 31) throw std::invalid_argument("Day must be an integer from 1 to 31.");
	if(year <= 1900 || year >= 2100) throw std::invalid_argument("Year must be an integer from 1901 to 2099.");
	if((month == 4 || month == 6 || month == 9 || month == 11) && day == 31) throw std::invalid_argument("Day is invalid for the given month.");
	if (month == 2) {
		if (day > 29) throw std::invalid_argument("Day is invalid for the given month.");
		else if (year % 4 == 0 && day == 29) throw std::invalid_argument("Day is invalid for the given month; this is not a leap year.");
	}
	this->month = month;
	this->day = day;
	this->year = year;
}

bool Date::equals(Date date) {
	if (this->year == date.year && this->month == date.month && this->day == date.day) return true;
	return false;
}
