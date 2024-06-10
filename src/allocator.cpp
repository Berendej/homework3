#include <utility>
#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <chrono>

#include "heap_alloc.h"
#include "simple_list.h"
using std::cout;
using std::cin;
using std::map;

constexpr int alloc_size{ 10 };

typedef heap_allocator<std::pair<const int, int>, alloc_size> map_alloc_t;
typedef std::map<int, int, std::less<int>, map_alloc_t >   my_map_t;

typedef heap_allocator<int, alloc_size> lst_alloc_t;
typedef simple_list_t<int, lst_alloc_t> my_lst_t;

int fact(int i)
{
	int res = 1;
	for ( int ix = 1; ix <= i ; ix++)
	{
		res *= ix;
	}
	return res;
}

void show_content(map<int, int> &smp, my_map_t &emp)
{
	cout << "emp content\n";
	auto eit = emp.begin();
	while ( eit != emp.end() ) 
	{
		cout << eit->second << " ";
		eit++;
	}
	cout << "\n";

	cout << "smp content\n";
	auto sit = smp.begin();
	while ( sit != smp.end() ) 
	{
		cout << sit->second << " ";
		sit++;
	}
	cout << "\n";
}
/*
- создание экземпляра std::map<int, int>
- заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
- создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10
элементами
- заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
- вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере

- создание экземпляра своего контейнера для хранения значений типа int
- заполнение 10 элементами от 0 до 9
- создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором,
ограниченным 10 элементами
- заполнение 10 элементами от 0 до 9
- вывод на экран всех значений, хранящихся в контейнере
*/
int main(int argc, char* argv[])
{
//- создание экземпляра std::map<int, int>
//- заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
// создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
// заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
	std::map<int, int> smp;
	my_map_t emp; 

	int ix;
	for(ix = 0; ix < alloc_size; ix++ )
	{
		smp[ix] = fact(ix);
		emp[ix] = fact(ix);
	}

	show_content(smp, emp);
/*
- создание экземпляра своего контейнера для хранения значений типа int
- заполнение 10 элементами от 0 до 9
- создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором,
  ограниченным 10 элементами
- заполнение 10 элементами от 0 до 9
- вывод на экран всех значений, хранящихся в контейнере
*/

	simple_list_t<int, std::allocator<int>> lst1;
	for( ix = 0; ix < alloc_size; ix++)
	{
		lst1.append(ix);
	}
	cout << "content of simple list std::allocator\n";
	auto it1 = lst1.begin();
	while ( it1 != lst1.end() )
	{
		std::cout << *it1 << " ";
		it1++;
	}


	my_lst_t lst2;
	for( ix = 0; ix < alloc_size; ix++)
	{
		lst2.append(ix);
	}
	cout << "\ncontent of list lst_alloc_t\n";
	auto it2 = lst2.begin();
	while ( it2 != lst2.end() )
	{
		std::cout << *it2 << " ";
		it2++;
	}

	cout << "\npress any key + enter to end\n";
	char ch;
	cin >> ch;
	return 0;
}

