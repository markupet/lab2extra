#include "grelian.h"
#include<time.h>

namespace lab2
{
	GreLian::GreLian()
	{
		time_t rawtime = k_time( 0 );
	
		struct tm * timeinfo = localtime ( &rawtime );
		
		_year = timeinfo->tm_year + 1900;
		_month = timeinfo->tm_mon + 1;
		_day = timeinfo->tm_mday;
	}
	
	GreLian::GreLian(int year, int month, int day)
	{
		_year = year;
		_month = month;
		_day = day;
	}
	
	GreLian::GreLian(const Date & date) : Date(date) {}
	
	Date & GreLian::operator=(const Date & date)
	{
		_year = date.year();
		_month = date.month();
		_day = date.day();
		
		*this -= mod_julian_day() - date.mod_julian_day();
		
		return *this;
	}
	
	GreLian & GreLian::operator=(const GreLian & date)
	{
		_year = date.year();
		_month = date.month();
		_day = date.day();
		
		*this -= mod_julian_day() - date.mod_julian_day();
		
		return *this;
	}
	
	int GreLian::days_per_week() const
	{
		return 7;
	}
	
	int GreLian::days_this_month() const
	{
		return days_per_month(_month);
	}
	
	int GreLian::months_per_year() const
	{
		return 12;
	}

	int GreLian::week_day() const
	{
		//get the julian day number (not modified)
		// JDmod = JD - 2400000.5

		// TODO: maybe add another day (+ 2400001) depending on time of day
		// see http://en.wikipedia.org/wiki/Modified_Julian_date
		// #Finding_day_of_week_given_Julian_day_number
		// for details.

		int j_date = mod_julian_day() + 2400001;
		return (j_date % 7) + 1;
	}
	std::string GreLian::week_day_name() const
	{
		return weekdays[week_day() - 1];
	}
	
	std::string GreLian::month_name() const
	{
		if (_month >= 1 && _month <= 12)
			return months[_month-1];
		std::cout << "month=" << _month << std::endl;
		throw std::out_of_range("Invalid month! month=");
	}
	
	//add one day
	Date& GreLian::operator++()
	{
		operator+=(1);
		return *this;
	}

	Date& GreLian::operator--()
	{
		operator-=(1);
		return *this;
	}
	
	//TODO: if n is huge, this will take a long time. May do it one year at a time instead
	void GreLian::operator+=(const int & n)
	{
		if (n < 0)
			return operator-=(-n);
		
		_day += n;
		while (_day > days_per_month(_month))
		{
			_day -= days_per_month(_month);
			++_month;
			if (_month > months_per_year())
			{
				_month = 1;
				++_year;
			}
		}
	}
	
	void GreLian::operator-=(const int & n)
	{
		if (n < 0)
			return operator+=(-n);
		
		_day -= n;
		while (_day < 1)
		{
			--_month;
			if (_month < 1)
			{
				_month = months_per_year();
				--_year;
			}
			_day += days_per_month(_month);
		}
	}
	
	void GreLian::add_month(int n)
	{
		if (n > 0)
			for (int i = 0; i < n; ++i)
				add_single_month();
		else
			for (int i = n; i < 0; ++i)
				remove_single_month();
	}
	
	void GreLian::add_single_month()
	{
		if (_day > days_per_month((_month % months_per_year()) + 1))
			return operator+=(30);
		++_month;
		if (_month > months_per_year())
		{
			++_year;
			_month = 1;
		}
	}
	
	void GreLian::remove_single_month()
	{
		if (_day > days_per_month(_month + (1/_month)*months_per_year() - 1))
			return operator-=(30);
		--_month;
		if (_month < 1)
		{
			--_year;
			_month = months_per_year();
		}
	}
	
	void GreLian::add_year(int n)
	{
		if (month() == 2 && day() == 29 && !is_leap_year(_year + n))
			_day = 28;
		_year += n;
	}
	
	int GreLian::days_per_month(int month) const
	{
		if (month < 1 || month > months_per_year())
			throw std::out_of_range("Invalid month.");
			
		if (month == 2 && is_leap_year(_year))
			return 29;
		return days_per_month_list[month-1];
	}
	
	bool GreLian::is_leap_day() const
	{
		return is_leap_year(_year) && _month == 2 && _day == 29;
	}
	
	const std::string GreLian::months[] =
	{ "january", "february", "march", "april", "may", "june",
	"july", "august", "september", "october", "november", "december"};
	
	const std::string GreLian::weekdays[] =
	{ "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday" };
	const int GreLian::days_per_month_list[] =
		{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
}
