#ifdef _WIN32
 #include "..\public\date.h"
#else
 #include "../public/date.h"
#endif
 

//Standard constructor
Date::Date(int year, int month, int day) {
	//assertion
	if(month < 1 || month > 12) throw std::invalid_argument("Month must be an integer from 1 to 12.");
	if(day < 1 || day > 31) throw std::invalid_argument("Day must be an integer from 1 to 31.");
	if(year <= 1900 || year >= 2100) throw std::invalid_argument("Year must be an integer from 1901 to 2099.");
	if((month == 4 || month == 6 || month == 9 || month == 11) && day == 31) throw std::invalid_argument("Day is invalid for the given month.");
	if (month == 2) {
		if (day > 29) throw std::invalid_argument("Day is invalid for the given month.");
		else if (year % 4 == 0 && day == 29) throw std::invalid_argument("Day is invalid for the given month; this is not a leap year.");
	}
	this->year = year;
	this->month = month;
	this->day = day;
}

//Takes a string of the form 2021-07-19 and makes it into a Date obbject.
Date::Date(std::string datestring) {

	year = stoi(datestring.substr(0, 4));
	month = stoi(datestring.substr(5, 2));
	day = stoi(datestring.substr(8, 2));
}

//See if two Date objects represent the same date.
bool Date::equals(Date date) {
	if (this->year == date.year && this->month == date.month && this->day == date.day) return true;
	return false;
}

//Takes a Date object and returns a strong of the form 2021-07-19.
std::string Date::tostring() {
	std::string daydash = "-";
	std::string monthdash = "-";
	if (day < 10) { daydash = "-0"; }
	if (month < 10) { monthdash = "-0"; }
	std::string stringbuilder = std::to_string(year) + monthdash + std::to_string(month) + daydash + std::to_string(day);
	return stringbuilder;
}
