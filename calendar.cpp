#include<string.h>
#include<list>
#include<iostream>
#include<type_traits>
#include<sstream>
#include<vector>

#include"date.h"
#include"grelian.h"

namespace lab2
{
	enum ReoccurenceType { Day, Week, Month, Year };
	template<typename T>
	struct Event
	{
		Event(const std::string e, const T d) : date(d), description(e), parent(nullptr) {}
		T date;
		std::string description;
		std::vector<Event<T> *> rel_events;
		Event<T> * parent;
		
		
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
		bool can_move_event(const Event<T> & event, int daystoadd) const;
		Event<T> * move_event(Event<T> & event, int daystoadd);
		bool remove_event_rec(Event<T> *);

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
		bool add_event(std::string, int day, int month, int year);
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
		Event<T> * find_event(const Date & date, std::string description);
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
	bool Calendar<T>::add_event(std::string desc, int day, int month, int year)
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
					events.emplace(it, desc, new_date);
					return true;
				}
			}
			//if we get here, add to end of list
			events.emplace(events.end(), desc, new_date);
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
		return add_event(event, date.day(), date.month(), date.year());
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
		//find the event to remove
		T date(year, month, day);
		Event<T> * ev = find_event(date, desc);
		if (ev == nullptr)
			return false;
			
		//remove parent's reference to this event
		if (ev->parent != nullptr)
			for (auto it = ev->parent->rel_events.begin(); it != ev->parent->rel_events.end(); ++it)
				if (*it == ev){
					ev->parent->rel_events.erase(it);
					break;
				}
		
		//remove event and all children
		return remove_event_rec(ev);
	}
	
	template<typename T>
	bool Calendar<T>::remove_event_rec(Event<T> * event)
	{
		for (Event<T> * rel_event : event->rel_events)
			remove_event_rec(rel_event);
	
		events.remove(*event);
		return true;
	}
	
	template<typename T>
	bool Calendar<T>::move_event(const Date & from, const Date & to, std::string event)
	{
		std::cout << "> move_event: " << event << std::endl;
		int daystoadd = to - from;
		
		//check if movement will result in collision for main events
		if (event_exists(to, event) || !(event_exists(from, event)))
			return false;
		
		Event<T> * event_to_move = find_event(from, event);
		
		//check if we can move everything
		if (!can_move_event(*event_to_move, daystoadd))
				return false;
		
		std::cout << "går att flytta\n";
		
		//move this event and all it's children (recursively)
		Event<T> * just_created = move_event(*event_to_move, daystoadd);
		
		std::cout << "slutfix...";
		//notify the father of the change
		if (just_created->parent != nullptr)
			just_created->parent->rel_events.push_back(just_created);
		std::cout << "klart.\n";
		return true;	
	}
	
	template<typename T>
	Event<T> * Calendar<T>::move_event(Event<T> & event, int daystoadd)
	{
		std::cout << "   > move_event " << event;
		//assuming that all events can be moved
		
		//create the new event and add it
		T new_date = event.date;
		new_date += daystoadd;
		add_event(event.description, new_date);
		
		//Find a pointer to the event just created
		Event<T> * ev = find_event(new_date, event.description);
		
		std::cout << "created " << *ev;
		
		//give the new event a father
		ev->parent = event.parent;
		
		//give the new event it's children
		while (event.rel_events.size() > 0){
			Event<T>* rel = event.rel_events[event.rel_events.size() - 1];
			event.rel_events.pop_back();
			
//		for (Event<T> * rel : event.rel_events) {
			//ändra förälder till nya eventet
			rel->parent = ev;
			std::cout << "   > " << event.description << ": ";
			Event<T> * ets = move_event(*rel, daystoadd);
			std::cout << "ets: " << *ets << std::endl;
			ev->rel_events.push_back(ets);
		}
		
		
		
		std::cout << "removing " << event.description << "... ";
		//remove the old event
		remove_event(event.description, event.date.day(), event.date.month(), event.date.year());

		std::cout << "done.\n";

		std::cout << *ev;
		
		return ev;
	}
	
	template<typename T>
	bool Calendar<T>::add_related_event(const Date & rel_date, int days, 
					std::string rel_event, std::string new_event)
	{
		// find related event (which will be the parent of the new event) if it exists
		Event<T> * rel = find_event(rel_date, rel_event);
		
		T date = rel_date;
		date += days;

		if (rel == nullptr || !add_event(new_event, date.day(), date.month(), date.year()))
			return false;
		//rel exists and new event could be added
		
		//find the newly created event
		Event<T> * ev = find_event(date, new_event);
		if (ev == nullptr) //something went wrong...
			return false;

		//add connection between parent and child
		rel->rel_events.push_back(ev);
		//add connection between child and parent
		ev->parent = rel;
		return true;
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
	Event<T> * Calendar<T>::find_event(const Date & date, std::string description)
	{
		for (auto it = events.begin(); it != events.end(); ++it)
		{
			if (it->date == date && it->description == description)
				return &(*it);
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
	bool Calendar<T>::can_move_event(const Event<T> & event, int daystoadd) const
	{
		T date = event.date;
		date += daystoadd;
		if (event_exists(date, event.description)){
			return false;
		}
		for (Event<T> * rel_event : event.rel_events){
			if (!can_move_event(*rel_event, daystoadd))
				return false;
		}
		return true;
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
