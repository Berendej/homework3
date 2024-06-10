#include <utility>
#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <chrono>

#define BOOST_TEST_MODULE homework3 test

#include <boost/test/included/unit_test.hpp>

#include "heap_alloc.h"
using std::cout;
using std::map;

constexpr int alloc_size{ 100 };

typedef std::map<int, int, std::less<int>, heap_allocator<std::pair<const int, int>, alloc_size > >  my_map_t;
typedef heap_allocator<std::pair<int, int>, alloc_size> my_alloc_t;

int gen_rand()
{
    static const int from = 0;
    static std::random_device rd; // obtain a random number from hardware
    static std::mt19937 gen(rd()); // seed the generator
    static std::uniform_int_distribution<> distr(0, alloc_size); // define the range
    return distr(gen);
}


template<typename map_type>
void insert_erase_test(map_type &mp)
{
	mp.clear();
	int ix;
	for(ix = 0; ix < alloc_size; ix++ )
	{
		mp[ix] = ix;
	}
	mp.clear();
	for(ix = 0; ix < alloc_size; ix++ )
	{
		mp[ix] = ix;
	}
	// randomly erase half elements
	ix = 0;
	while(ix < alloc_size/2)
	{
		int erase_ix = gen_rand();
		if ( mp.find(erase_ix) != mp.end() )
		{
			mp.erase(erase_ix);
			ix++;
		};
	}
	// randomly insert 1/2 alloc size
	// starting from tail
	ix = alloc_size;
	while(ix > (alloc_size/2) )
	{
		if ( mp.find(ix) == mp.end() )
		{
			// if it was erased
			mp[ix] = ix;
		}
		ix--;
	}
}

///////////////////////////////////////////////////////////
// borrowed code 
template <typename map_type>
auto benchmark(map_type mp, my_alloc_t &ea, int iterations) {
	ea.reset();
    const auto start = std::chrono::system_clock::now();
    while (iterations-- > 0) { insert_erase_test(mp); }
    const auto stop = std::chrono::system_clock::now();
    const auto secs = std::chrono::duration<double>(stop - start);
    return secs.count();
}

void compare_allocs()
{
    constexpr int iterations{1000};
	my_alloc_t ea;
	my_map_t emp; (ea); 
	std::map<int, int> smp;

    const double t1 = benchmark(smp, ea, iterations);
    const double t2 = benchmark(emp, ea, iterations);
     std::cout << std::fixed
              << "t1 (default std alloc): " << t1 << " sec;" << '\n'
              << "t2 (effective?? alloc): " << t2 << " sec;" << '\n';
}
///////////////////////////////////////////////////////////

bool test_heap_allocator()
{
	my_map_t emp;
	std::map<int, int> smp;
	int ix;
	for(ix = 0; ix < alloc_size; ix++ )
	{
		emp[ix] = ix; 	smp[ix] = ix;
	}
	emp.clear();
	for(ix = 0; ix < alloc_size; ix++ )
	{  // once again:
		emp[ix] = ix;
	}
	// randomly erase half elements
	ix = 0;
	while(ix < alloc_size/2)
	{
		int erase_ix = gen_rand();
		if ( emp.find(erase_ix) != emp.end() )
		{
			emp.erase(erase_ix); smp.erase(erase_ix);
			ix++;
		};
	}
	// randomly insert 1/2 alloc size
	// starting from tail
	ix = alloc_size;
	while(ix > (alloc_size/2)) 
	{
		if ( smp.find(ix) == smp.end() )
		{
			// if it was previously erased
			smp[ix] = ix; emp[ix] = ix;
		}
		ix--;
	}
	if ( emp.size() != smp.size() )
	{
		cout << "different sizes std " << smp.size() 
		                    << " emp " << emp.size() << "\n" ;
		return false;
	}

	for (int ix = 0; ix < alloc_size; ix++)
	{
		if (smp.find(ix) != smp.end() )
		{
			if (emp.find(ix) == emp.end() )
			{
				cout << ix << " ERROR  not found in emp\n";
				return false;
			}
			else
			{
				if ( emp[ix] != ix )
				{
					cout << "ERROR emp[" << ix << "] = " << emp[ix] << "\n";
					return false;
				}
				if ( smp[ix] != ix )
				{
					cout << "ERROR smp[" << ix << "] = " << smp[ix] << "\n";
					return false;
				}
			}
		}
	}
	cout << "Seems like it works\n";
	return true;
}

BOOST_AUTO_TEST_CASE( heap_alloc_test )
{
    BOOST_CHECK( test_heap_allocator()  );
}
