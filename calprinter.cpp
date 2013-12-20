#pragma once

#include "calendar.cpp"
#include <ostream>

namespace lab2
{
	template<typename T>
	std::ostream & operator<<(std::ostream & os, const Calendar<T> & cal)
	{
		auto events = cal.get_events();
		
		for(auto & event : events)
			if (event.date > cal.get_current_date())
				os << event.date << " : " << event.description << std::endl;
		
		return os;
	}
	
	template<typename T>
	std::ostream & operator<<(std::ostream & os, const Event<T> & e)
	{
		os << e.date << " : " << e.description << std::endl;
		return os;
	}


}
