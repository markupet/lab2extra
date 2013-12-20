#pragma once

#include<iostream>

#include "grelian.h"

namespace lab2
{
	class Julian : public GreLian
	{
		bool is_leap_year(int year) const;
		
	public:
		Julian();
		Julian(int year, int month, int day);
		Julian(const Date & date);
		Julian(Date* & dp);
		
		int mod_julian_day() const;
	};
}
