#pragma once

#include<string>
#include<iostream>
#include<stdexcept>
#include "date.h"
#include<typeinfo>

namespace lab2
{
	class GreLian : public Date
	{
		const static std::string months[];
		const static std::string weekdays[];
		const static int days_per_month_list[];
		
		virtual bool is_leap_year(int year) const = 0;
		void add_single_month();
		void remove_single_month();
		
	protected:
		int days_per_month(int month) const;

	public:
		GreLian();
		GreLian(int year, int month, int day);
		GreLian(const Date & date);
		
		Date & operator=(const Date & date);
		GreLian & operator=(const GreLian & date);
		
		int days_per_week() const;
		int days_this_month() const;
		int months_per_year() const;
	
		int week_day() const;
		std::string week_day_name() const;
		std::string month_name() const;
		
		//mutatorer
		Date & operator++();
		Date & operator--();
		void operator+=(const int & n);
		void operator-=(const int & n);
		
		void add_month(int n = 1);
		void add_year(int n = 1);
		
		bool is_leap_day() const;
	};
}
