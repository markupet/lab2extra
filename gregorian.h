#pragma once

#include<iostream>

#include "grelian.h"

namespace lab2
{
	class Gregorian : public GreLian
	{
		bool is_leap_year(int year) const;
		
	public:
		Gregorian();
		Gregorian(int year, int month, int day);
		Gregorian(const Date & date);
		Gregorian(Date * & dp);
		
		int mod_julian_day() const;
		
		bool check_leap_year(int year) const;
	};
}
