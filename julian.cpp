#include"julian.h"

#include<cstdio>

using namespace std;

namespace lab2
{
	Julian::Julian() : GreLian()
	{
		int centuries = (_year - 1800) / 100;
		// possible values: 0, 1, ..., 7
		// mappings: 0 -> 12, 1 -> 13, 2 -> 13, 3 -> 14, 4 -> 15, 5 -> 16, 6 -> 16, 7 -> 17
		*this -= 12 + centuries - (centuries + 2) / 4;
	}
	
	Julian::Julian(int year, int month, int day) : GreLian(year, month, day)
	{
		if (month < 1 || month > months_per_year())
			throw std::out_of_range("Month is invalid!\n");
		if (year < 1858 || year > 2558)
			throw std::out_of_range("Year is invalid!\n");
		if (day < 1 || day > days_per_month(month))
			throw std::out_of_range("Day is invalid!\n");
	}
	
	Julian::Julian(const Date & date) : GreLian(date)
	{
		int diff = mod_julian_day() - date.mod_julian_day();
		*this -= diff;
	}

	Julian::Julian(Date* & dp) : GreLian(*dp)
	{
		int diff = mod_julian_day() - dp->mod_julian_day();
		*this -= diff;
	}
		
	bool Julian::is_leap_year(int year) const
	{
		return year % 4 == 0;
	}
	
	int Julian::mod_julian_day() const
	{
		int a = (14 - _month) / 12;
		int y = _year + 4800 - a;
		int m = _month + 12 * a - 3;
		int jdn = _day + (153*m + 2)/5 + 365*y + y/4 - 32084; //32083
		return jdn - 2400000;
	}
}

/*
#include<cstdio>
using namespace lab2;

int main()
{
	//set kattis time
	time_t rawtime;
	time( &rawtime );
	set_k_time( rawtime );
	
	Gregorian g;
	std::cout << g.week_day_name() << std::endl;
}*/
