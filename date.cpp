#include "date.h"

namespace lab2
{
	Date::Date() {}
	
	Date::Date(const Date & date) : 
		_year(date.year()), _month(date.month()), _day(date.day())	{}
	
	int Date::year() const
	{
		return _year;
	}

	int Date::month() const
	{
		return _month;
	}
	
	int Date::day() const
	{
		return _day;
	}
	
	void Date::add_year(int n)
	{
		_year += n;
	}
	
	void Date::add_month(int n)
	{
		_month += n;
	}
	
	bool Date::operator==(const Date & d) const
	{
		return mod_julian_day() == d.mod_julian_day();
	}
	
	bool Date::operator!=(const Date & d) const
	{
		return mod_julian_day() != d.mod_julian_day();
	}
	
	bool Date::operator<(const Date & d) const
	{
		return mod_julian_day() < d.mod_julian_day();
	}
	
	bool Date::operator<=(const Date & d) const
	{
		return mod_julian_day() <= d.mod_julian_day();
	}
	
	bool Date::operator>(const Date & d) const
	{
		return mod_julian_day() > d.mod_julian_day();
	}
	
	bool Date::operator>=(const Date & d) const
	{
		return mod_julian_day() >= d.mod_julian_day();
	}
	
	int Date::operator-(const Date & d) const
	{
		return mod_julian_day() - d.mod_julian_day();
	}
	
	std::ostream & operator<<(std::ostream & os, const Date & date)
	{
		//YYYY-MM-DD
		if (date.year() < 10)
			os << '0';
		if (date.year() < 100)
			os << '0';
		if (date.year() < 1000)
			os << '0';
		os << date.year() << '-';
		if (date.month() < 10)
			os << '0';
		os << date.month() << '-';
		if (date.day() < 10)
			os << '0';
		os << date.day();

		return os;
	}	

}

