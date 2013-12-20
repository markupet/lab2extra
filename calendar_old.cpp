#include<string.h>
#include<list>
#include<iostream>
#include<type_traits>
#include<sstream>

#include"date.h"
#include"grelian.h"

namespace lab2
{
	enum ReoccurenceType { Day, Week, Month, Year };
	template<typename T>
	struct Event
	{
		Event(const std::string e, const T d, Event<T> * r = nullptr) 
			: date(d), description(e), rel_event(r) {}
		T date;
		std::string description;
		Event<T> * rel_event;
		
		
		bool operator==(const Event<T> & event)
		{
			return date == event.date && description == event.description;
		}
		
		bool operator!=(const Event<T> & event)
		{
			return !operator==(event);
		}
	};
	
	template<typename T>
	class Calendar
	{
	public:
		enum format { list, cal, iCalendar };
	private:
		format output_format;
		std::list<Event<T>> events;
		T current_date;
		//int _year, _month, _day;

		void insert_event(const Event<T> & event);
		bool add_recurring_event(const std::string & event, T start,
							ReoccurenceType type, uint noccurences, bool is_birthday);
		
	public:
		Calendar();
		
		template<typename S>
		Calendar(const Calendar<S> & cal);
		
		template<typename S>
		Calendar<T> & operator=(const Calendar<S> & cal);
		
		bool set_date(int year, int month, int day);
		
		bool add_event(std::string);
		bool add_event(std::string, int day);
		bool add_event(std::string, int day, int month);
		bool add_event(std::string, int day, int month, int year, Event<T> * rel_event = nullptr);
		bool add_event(const std::string & event, const T & date);
		
		bool remove_event(std::string);
		bool remove_event(std::string, int day);
		bool remove_event(std::string, int day, int month);
		bool remove_event(std::string, int day, int month, int year);

		const std::list<Event<T>> & get_events() const;
		const T & get_current_date() const;
		
		// Extrauppgift 2.1
		bool move_event(const Date & from, const Date & to, std::string event);
		bool add_related_event(const Date & date, int days, std::string rel_event,
							   std::string new_event);
		bool event_exists(const Date & date, std::string description) const;
		Event<T> * find_event(const Date & date, std::string description) const;
		bool add_recurring_event(const std::string & event, const T & start, 
								ReoccurenceType type, uint noccurences = 100);
		bool add_birthday(const std::string & name, const T & date_of_birth);
		
		// Extrauppgift 2.2
		void set_format(const format & newformat);
		format get_print_format() const;
		void busy_days(int year, int month, bool * days) const;
	};
	
	template<typename T>
	std::ostream & operator<<(std::ostream & os, const Calendar<T> & cal);

	template<typename T>
	std::ostream & operator<<(std::ostream & os, const Event<T> &);
	
	////////////////////////////////////////////////////////////////
	////////////////// IMPLEMENTATION STARTS HERE //////////////////
	////////////////////////////////////////////////////////////////

	
	template<typename T>
	Calendar<T>::Calendar() : output_format(list) {}
	
	template<typename T>
	template<typename S>
	Calendar<T>::Calendar(const Calendar<S> & cal) : output_format(cal.output_format)
	{
		events.clear();
		for(auto event : cal.get_events())
			events.emplace_back(event.description, event.date);
		current_date = cal.get_current_date();

	}
	
	template<typename T>
	template<typename S>
	Calendar<T> & Calendar<T>::operator=(const Calendar<S> & cal)
	{
		events.clear();
		for(auto event : cal.get_events())
			events.emplace_back(event.description, event.date);
		current_date = cal.get_current_date();
		return *this;
	}
		
	template<typename T>
	bool Calendar<T>::set_date(int year, int month, int day)
	{
		try
		{
			current_date = T(year, month, day);
		}
		catch (const std::out_of_range &)
		{
			return false;
		}
		return true;
	}
		
	template<typename T>
	bool Calendar<T>::add_event(std::string desc)
	{
		return add_event(desc, current_date.day(), current_date.month(), 
			current_date.year());
	}
	
	template<typename T>
	bool Calendar<T>::add_event(std::string desc, int day)
	{
		return add_event(desc, day, current_date.month(), current_date.year());
	}
	
	template<typename T>
	bool Calendar<T>::add_event(std::string desc, int day, int month)
	{
		return add_event(desc, day, month, current_date.year());
	}
	
	template<typename T>
	bool Calendar<T>::add_event(std::string desc, int day, int month, int year,
		Event<T> * rel_event)
	{
		try
		{
			T new_date(year, month, day);

			for (auto it = events.begin(); it != events.end(); ++it)
			{
				if (it->date == new_date) //look for identical event
				{
					if (it->description == desc) //identical event found
						return false;
					//else, go to next event
				}
				else if(it->date > new_date) //insert new element before it
				{
					events.emplace(it, desc, new_date, rel_event);
					return true;
				}
			}
			//if we get here, add to end of list
			events.emplace(events.end(), desc, new_date, rel_event);
			return true;
		}
		catch (const std::out_of_range &)
		{
			return false;
		}
	}
	
	template<typename T>
	bool Calendar<T>::add_event(const std::string & event, const T & date)
	{
		add_event(event, date.day(), date.month(), date.year());
	}
	
	template<typename T>
	bool Calendar<T>::remove_event(std::string desc)
	{
		return remove_event(desc, current_date.day(), current_date.month(), 
			current_date.year());
	}
	
	template<typename T>
	bool Calendar<T>::remove_event(std::string desc, int day)
	{
		return remove_event(desc, day, current_date.month(), current_date.year());
	}
	
	template<typename T>
	bool Calendar<T>::remove_event(std::string desc, int day, int month)
	{
		return remove_event(desc, day, month, current_date.year());
	}
	
	template<typename T>
	bool Calendar<T>::remove_event(std::string desc, int day, int month, int year)
	{
		try
		{
			T date(year, month, day);
			for(auto it = events.begin(); it != events.end(); ++it)
				if (it->date == date && it->description == desc)
				{
					if (it->rel_event == nullptr)
					{
						events.erase(it);
						return true;
					}
					Event<T> * rel_event = it->rel_event;
					events.erase(it);
					rel_event->rel_event = nullptr;
					return remove_event(rel_event->description, rel_event->date.day(),
							rel_event->date.month(), rel_event->date.year());
				}
				else if (it->date > date)
					return false;
			return false;
		}
		catch (const std::out_of_range &)
		{
			return false;
		}
	}
	
	template<typename T>
	bool Calendar<T>::move_event(const Date & from, const Date & to, std::string event)
	{
		//check if movement will result in collision
		if (event_exists(to, event) || !(event_exists(from, event)))
			return false;
		
		//find the event to be moved
		for (Event<T> e : events)
		{
			if (e.date != from || e.description != event)
				continue;
			//correct element found, stored as e
			if (e.rel_event == nullptr) //unrelated event
			{
				remove_event(event, from.day(), from.month(), from.year());
				add_event(event, to.day(), to.month(), to.year());
				return true;
			}
			//else, related event. Oh crap...
			//assuming that the related event exists
			
			// check that the related event can be moved
			int diff = e.rel_event->date - e.date;
			T new_rel_date = to;
			new_rel_date += diff;
			
			if (event_exists(new_rel_date, e.rel_event->description)) //collision
				return false;
			
			//so, let's move stuff around...
			std::string rel_desc = e.rel_event->description;
			
			remove_event(event, from.day(), from.month(), from.year());
			remove_event(e.rel_event->description, e.rel_event->date.day(),
						e.rel_event->date.month(), e.rel_event->date.year());
						
			add_related_event(to, diff, rel_desc, event);
			return true;
		}
		return false;
	}
	
	template<typename T>
	bool Calendar<T>::add_related_event(const Date & date, int days, 
					std::string rel_event, std::string new_event)
	{
		T rel_date = date;
		rel_date += days;
		if (event_exists(date, new_event) || event_exists(rel_date, rel_event))
			return false;
		
		add_event(new_event, date.day(), date.month(), date.year());
		add_event(rel_event, rel_date.day(), rel_date.month(), rel_date.year());
		
		auto it_first = events.end();
		auto it_second = it_first;

		//find iterators		
		for (auto it = events.begin(); it != events.end(); ++it)
		{
			if (it->date == date && it->description == new_event && it_first == events.end())
				it_first = it;
			else if (it->date == rel_date && it->description == rel_event && it_second == events.end())
				it_second = it;
			if (it_first != events.end() && it_second != events.end())
				break;
		}
		it_first->rel_event = &*it_second;
		it_second->rel_event = &*it_first;
		
		return true;
		/*		
		Event<T> e(new_event, date);
		
		Event<T> e2(rel_event, rel_date, &e);
		e.rel_event = &e2;
		
		insert_event(e);
		insert_event(e2);*/
	}
	
	template<typename T>
	bool Calendar<T>::event_exists(const Date & date, std::string description) const
	{
		for (auto it = events.begin(); it != events.end(); ++it)
			if (it->date == date && it->description == description)
				return true;
			else if (it->date > date)
				return false;
		return false;
	}
	
	template<typename T>
	Event<T> * Calendar<T>::find_event(const Date & date, std::string description) const
	{
		for (Event<T> & e : events)
		{
			if (e.date == date && e.description == description)
				return &e;
		}
		return nullptr;
	}
	
	template<typename T>
	void Calendar<T>::insert_event(const Event<T> & event)
	{
		for (auto it = events.begin(); it != events.end(); ++it)
			if (it->date > event.date)
			{
				events.insert(it, event);
				std::cout << "Event '" << event.date << "' inserted\n";
				return;
			}
		events.push_back(event);
	}
	
	template<typename T>
	std::string create_birthday(const std::string & name, const T & now, const T & birth)
	{
		int age = now.year() - birth.year();
		std::stringstream ss;
		if (0 == age)
			ss << name << " föddes!";
		else
		{
			ss << name << " fyller ";
			ss << age;
			ss << " år";
		}
		return ss.str();
	}
	
	template<typename T>
bool Calendar<T>::add_recurring_event(const std::string & event, const T & date, 							ReoccurenceType type, uint noccurences)
	{
		return add_recurring_event(event, date, type, noccurences, false);
	}
	
	// Adds noccurences copies of the specified event each day, week, month or year.
	// For each collision with existing event, the number of copies will be reduced by one
	// Returns false if any copy collided with another event, true otherwise.
	template<typename T>
bool Calendar<T>::add_recurring_event(const std::string & event, T date, 							ReoccurenceType type, uint noccurences, bool birthday)
	{
		bool ret = true;
		int n = 0;
		switch(type)
		{
			case Year:
			{
				T base_date = date;
				if (std::is_base_of<GreLian, T>::value && date.is_leap_day())
				{
					for(int i = 0; i < noccurences; ++i)
					{
						if (i % 4 == 0)
						{
							date = base_date;
							date.add_year(i);
						}
						if (birthday)
							ret = ret && add_event(create_birthday(event, date, base_date), date);
						else
							ret = ret && add_event(event, date);
						date.add_year();
					}
					
				}
				else
					for(int i = 0; i < noccurences; ++i)
					{
						if (birthday)
							ret = ret && add_event(create_birthday(event, date, base_date), date);
						else
							ret = ret && add_event(event, date);
						date.add_year();
					}
				return ret;
			}
			case Month:
			{
				for(int i = 0; i < noccurences; ++i)
				{
					ret = ret && add_event(event, date);
					date.add_month();
				}
				return ret;
			}
			case Week:
			{
				n = 7;
				break;
			}
			case Day:
			{
				n = 1;
				break;
			}
		}
		for (int i = 0; i < noccurences; ++i)
		{
			ret = ret && add_event(event, date);
			date += n;
		}
		return ret;
	}
	
	template<typename T>
	bool Calendar<T>::add_birthday(const std::string & name, const T & date_of_birth)
	{
		return add_recurring_event(name, date_of_birth, Year, 100, true);
	}
	
	template<typename T>
	const std::list<Event<T>> & Calendar<T>::get_events() const
	{
		return events;
	}
	
	template<typename T>
	const T & Calendar<T>::get_current_date() const
	{
		return current_date;
	}
	
	///////////////////////////////////////////////////////////
	//////////////// Extrauppgift 2.2 /////////////////////////
	///////////////////////////////////////////////////////////
	template<typename T>
	void Calendar<T>::set_format(const format & newformat)
	{
		output_format = newformat;
	}
	
	template<typename T>
	typename Calendar<T>::format Calendar<T>::get_print_format() const
	{
		return output_format;
	}
	
	template<typename T>
	void Calendar<T>::busy_days(int year, int month, bool * days) const
	{
		for (auto event : events)
		{
			if (event.date.year() == year && event.date.month() == month)
				days[event.date.day() - 1] = true;
			else if (event.date.year() > year || 
					(event.date.year() == year && event.date.month() > month))
				break;
		}
	}

	template<typename T>
	std::ostream & list_output(std::ostream & os, const Calendar<T> & cal)
	{
		auto events = cal.get_events();
		
		for(auto & event : events)
			if (event.date > cal.get_current_date())
				os << event.date << " : " << event.description << std::endl;
		
		return os;
	}
	
	template<typename T>
	std::ostream & cal_output(std::ostream & os, const Calendar<T> & cal)
	{
		std::stringstream ss;
		const T now = cal.get_current_date();
		const T first_this_month(now.year(), now.month(), 1);
		
		//print month
		ss << now.month_name() << " " << now.year();
		std::string header = ss.str();
		
		int l = header.length();
		
		int leading_spaces = (5 * now.days_per_week() - l) / 2; //assuming 4 digits year
		for (int i = 0; i < leading_spaces; ++i)
			os << " ";
		os << header << std::endl;
		
		//print day names
		int weekday = now.week_day();
		T first_day = now;
		first_day -= weekday - 1;
		for (int i = 0; i < now.days_per_week(); ++i)
		{
			os << " " << first_day.week_day_name().substr(0,2) << "  ";
			first_day += 1;
		}
		os << std::endl;	
		
		bool days_with_events[now.days_this_month()];
		for (int i = 0; i < now.days_this_month(); ++i)
			days_with_events[i] = false;
			
		cal.busy_days(now.year(), now.month(), days_with_events);
		
		//print days
		
		int colindex = 0;
		for (int i = 2 - first_this_month.week_day(); i <= now.days_this_month(); ++i)
		{
			if (i < 1)
				os << "     ";
			else
			{
				os << (i == now.day() ? "<" : " ");
				os << (i < 10 ? " " : "") << i;
				
				if (i == now.day() && days_with_events[i-1])
					os << "*>";
				else if (i == now.day())
					os << "> ";
				else if (days_with_events[i-1])
					os << "* ";
				else
					os << "  ";
			}
			
			if (++colindex % 7 == 0)
				os << std::endl;
		}
		os << std::endl << std::endl;
		
		//print all events this month
		auto events = cal.get_events();
		for (auto event : events)
			if (event.date.year() == now.year() && event.date.month() == now.month())
				os << "  " << event;
			else if (event.date.year() > now.year() ||
					(event.date.year() == now.year() && event.date.month() > now.month()))
				break;
		
		return os;
	}
	
	template<typename T>
	std::ostream & iCalendar_output(std::ostream & os, const Calendar<T> & cal)
	{
		os << "BEGIN:VCALENDAR\n";
		os << "VERSION:2.0\n";
		os << "PRODID:MaLiCa, copyright 2013\n";
		auto events = cal.get_events();
		for (auto event : events)
		{
			os << "BEGIN:VEVENT\n";
			
			std::stringstream ss;
			ss << event.date.year();
			ss << (event.date.month() < 10 ? "0" : "") << event.date.month();
			ss << (event.date.day() < 10 ? "0" : "") << event.date.day();
			
			os << "DTSTART:" << ss.str() << "T080000\n";
			os << "DTEND:" << ss.str() << "T090000\n";
			os << "SUMMARY:" << event.description << std::endl;
			
			os << "END:VEVENT\n";
		}
		os << "END:VCALENDAR\n";
		return os;
	}

	template<typename T>
	std::ostream & operator<<(std::ostream & os, const Calendar<T> & cal)
	{
		typename Calendar<T>::format f = cal.get_print_format();
		switch (f)
		{
		case Calendar<T>::list: 		return list_output(os, cal);
		case Calendar<T>::cal:			return cal_output(os, cal);
		case Calendar<T>::iCalendar:	return iCalendar_output(os, cal);
		}
		return os;
	}
	
	template<typename T>
	std::ostream & operator<<(std::ostream & os, const Event<T> & e)
	{
		os << e.date << " : " << e.description << std::endl;
		return os;
	}

}

		//Tolkning av extrauppgift 2.1:
		// * Om new_event redan finns på datum date, eller 
		//		rel_event redan finns på datum date+days, returnera false
		// * Skapa new_event vid datum date
		// * Skapa rel_event vid datum date+days
		// * Lägg in relationen. rel_event beror på new_event, men inte tvärtom
		// 		- Om new_event ändras så ändras rel_event
		//			Specialfall: om rel_event ska flyttas och krockar med annat event flyttas inget av dem
