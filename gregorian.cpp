#include"gregorian.h"

#include<cstdio>
#include<iostream>

using namespace std;

namespace lab2
{
	Gregorian::Gregorian(int year, int month, int day) : GreLian(year, month, day)
	{
		if (month < 1 || month > months_per_year())
			throw std::out_of_range("Month is invalid!\n");
		if (year < 1858 || year > 2558)
			throw std::out_of_range("Year is invalid!\n");
		if (day < 1 || day > days_per_month(month))
			throw std::out_of_range("Day is invalid!\n");
	}
	
	Gregorian::Gregorian() : GreLian() {}
	Gregorian::Gregorian(const Date & date) : GreLian(date)
	{
		int diff = mod_julian_day() - date.mod_julian_day();
		*this -= diff;
	}

	Gregorian::Gregorian(Date * & dp) : GreLian(*dp)
	{
		int diff = mod_julian_day() - dp->mod_julian_day();
		*this -= diff;
	}
	
	bool Gregorian::is_leap_year(int year) const
	{
		return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
	}
	
	int Gregorian::mod_julian_day() const
	{
		int a = (14 - _month) / 12;
		int y = _year + 4800 - a;
		int m = _month + 12 * a - 3;
		int jdn = _day + (153*m + 2)/5 + 365*y + y/4 - y/100 + y/400 - 32046; //32045
		return jdn - 2400000;
	}
	
	bool Gregorian::check_leap_year(int year) const
	{
		if (is_leap_year(year))
			printf("%d is a leap year.\n", year);
		else
			printf("%d is not a leap year.\n", year);
	}
}

using namespace lab2;

void pm(Date & d)
{
	cout << d.month_name() << ' ' << d.day() << endl;
}

/*
int main()
{
	//set kattis time
	time_t rawtime;
	time( &rawtime );
	set_k_time( rawtime );
	
	Gregorian g(2013, 11, 2);
	//std::cout << g.week_day_name() << std::endl;
	pm(g);
	g.add_month(); // 13-12-2
	pm(g);
	g += 29; // 13-12-31
	g.add_month(-1); // 13-12-1
	pm(g);
	g += -1; // 13-11-30
	g.add_month(2);
	g += 1;
	pm(g); // 14-1-31
	g.add_month(); // 14-3-2
	pm(g);
	
	Gregorian g3(2012, 1, 31);
	g3.add_month(); // 12-3-1
	pm(g3);
	g3.add_month(-1);
	g3 -= 2; //12-1-30
	pm(g3);
	g3.add_month(); //12-2-29
	pm(g3);
	
	Gregorian g2(2012, 2, 29);
	g2.add_year();
	pm(g2);
	g2.add_year(-1);
	g2 += 1; // 12-2-29
	g2.add_year(4); //16-2-29
	pm(g2);
	g2.add_year(-116); //1900-2-28
	pm(g2);
	
//	int years[] = { 1600, 1601, 1700, 1900, 1991, 1992, 2000, 2010, 2012, 2013 };
	
//	for (int i = 0; i < 10; ++i)
//		g.check_leap_year(years[i]);
}*/
