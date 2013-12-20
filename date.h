#include<string>
#include<iostream>
#include "kattistime.h"

#pragma once

namespace lab2
{
	class Date
	{
	protected:
		int _year;
		int _month;
		int _day;
		
	public:
		Date();
		Date(const Date & date);
		
		virtual Date & operator=(const Date & date) = 0;
		
		int year() const;
		int month() const;
		int day() const;
		virtual int week_day() const = 0;
		virtual int days_per_week() const = 0;
		virtual int days_this_month() const = 0;
		virtual int months_per_year() const = 0;
	
		virtual std::string week_day_name() const = 0;
		virtual std::string month_name() const = 0;
		
		//mutatorer
		virtual Date & operator++() = 0;
		virtual Date & operator--() = 0;
		virtual void operator+=(const int & n) = 0;
		virtual void operator-=(const int & n) = 0;
		
		virtual void add_year(int n = 1);
		virtual void add_month(int n = 1);
		
		//jämförelser
		bool operator==(const Date & d) const;
		bool operator!=(const Date & d) const;
		bool operator<(const Date & d) const;
		bool operator<=(const Date & d) const;
		bool operator>(const Date & d) const;
		bool operator>=(const Date & d) const;
		int operator-(const Date & d) const;
		
		virtual int mod_julian_day() const = 0;
		
	};
	
	//utskrifter
	std::ostream & operator<<(std::ostream & os, const Date & date); //år-månad-dag		
}
