#include "calendar.cpp"
#include "gregorian.h"
#include "date.h"
#include<stdlib.h>
#include<cstdio>


using namespace std;
using namespace lab2;

void streck()
{
	std::cout << "-----------------------------\n";
}

void pbool(const bool b)
{
	cout << (b ? "TRUE" : "FALSE") << endl;
}

void wait_and_clear();

void new_test(const string & test)
{
	wait_and_clear();
	cout << endl;
	streck();
	streck();
	cout << "Testar nu: " << test << endl;
	streck();
	cout << endl;
}

template<typename T>
void streck(const Calendar<T> & c)
{
	cout << endl;
	streck();
	cout << c;
	streck();
	cout << endl;
}

void wait_and_clear();
void test_move_event();
void test_related_event();
void test_reoccurring_event();
void test_add_birthday();
void test_output();

int main()
{
	//set kattis time
	time_t rawtime;
	time( &rawtime );
	set_k_time( rawtime );
	system("clear");
	
	
	new_test("Move event");
	test_move_event();
	new_test("Related event");
	test_related_event();
	new_test("Reoccurring event");
	test_reoccurring_event();
	new_test("Birthday");
	test_add_birthday();
	new_test("Output");
	test_output();
}

void test_output()
{	
	Calendar<Gregorian> c;
	c.set_format(Calendar<Gregorian>::format::cal);
	c.add_event("hej", 1);
	c.add_event("hej igen");
	c.add_event("ajsblkj", 3, 12);
	cout << c;
	
	c.set_format(Calendar<Gregorian>::format::iCalendar);
	streck(c);
}

void test_add_birthday()
{
	Calendar<Gregorian> c;
	c.set_date(2012, 01, 15);
	
	Gregorian d1(2012, 02, 29);
	c.add_birthday("OtursNisse", d1);
	streck(c);
}

void test_reoccurring_event()
{
	Calendar<Gregorian> c;
	Gregorian d1(2012, 01, 15);
	c.set_date(2012, 01, 15);
	
	cout << "Current date is " << d1 << endl;
	
	d1 += 1;
	c.add_recurring_event("Markus fyller år", d1, Day, 5);
	streck(c);
	
	d1 += 1;
	cout << "Linus fyller bara år varje månad.\n";
	c.add_recurring_event("Linus fyller år, hurra!", d1, Month, 10);
	streck(c);
	
	Gregorian d2(2012, 02, 29);
	cout << "KallePelle fyller år på skottdagen.\n";
	c.add_recurring_event("KallePelles födelsedag", d2, Year, 10);
	streck(c);
	
	cout << "Ny kalender!\n";
	Calendar<Gregorian> c2;
	c2.set_date(1996, 01, 01);
	Gregorian g(1996, 02, 29);
	c2.add_recurring_event("Nån fyller år", g, Year,150);
	streck(c2);
}

void test_related_event()
{
	Calendar<Gregorian> c;
	Gregorian d1(2012, 01, 15);
	c.set_date(2011, 01, 15);
	
	cout << "Current date is " << d1 << endl;
	
	cout << "\nAdding related events\n";
	d1 += 1;
	c.add_event("party", d1);
	c.add_related_event(d1, -1, "party", "förfest");
	
	Gregorian d78(2012, 03, 25);
	c.add_event("random", d78);
	
	streck(c);
	
	Gregorian d10(2012, 01, 15);
	c.add_related_event(d10, -1, "förfest", "handla");
	c.add_related_event(d10, 2, "förfest", "städa");
	c.add_related_event(d10, 7, "förfest", "förfestens förfest");
	streck(c);
	
	cout << "Förfesten var ju på tok för tidigt...\n";
	Gregorian d11 = d10;
	d11 += 3;
	c.move_event(d1, d11, "party");
	cout << "number of events: " << c.get_events().size() << endl;
	streck(c);
}

void test_move_event()
{
	Calendar<Gregorian> c2;
	c2.set_date(2012, 10, 15);
	Gregorian ga(2012, 10, 15);
	cout << "New empty calendar, current date " << ga << endl;
	ga += 1;
	
	c2.add_event("ska flyttas", ga);
	streck(c2);
	
	Gregorian gb = ga;
	gb += 10;

	c2.move_event(ga, gb, "ska flyttas");
	cout << "Flyttat 'ska flyttas' till " << gb << endl;
	streck(c2);
	
	cout << "Försöker flytta ej existerande event\n";
	cout << (c2.move_event(ga, gb, "tja2") ? "true" : "false") << endl;
	streck(c2);
	
	c2.add_event("ska flyttas", ga);
	cout << "Added event\n";
	streck(c2);
	
	cout << "Försöker flytta, men kollision på target-datum\n";
	cout << (c2.move_event(ga, gb, "ska flyttas") ? "true" : "false") << endl;
	streck(c2);
}

void wait_and_clear()
{
	cout << "For next test, write something and press <Enter>...";
	string blabla;
	cin >> blabla;
	system("clear");
}
