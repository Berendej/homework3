#ifndef _HEAP_ALLOC_H_
#define _HEAP_ALLOC_H_

#include <memory>
#include <utility>
#include <cassert>
#include <cstdint>
#include <iostream>

using std::cout;

template<class T, std::size_t sz>
struct heap_allocator
{
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;

	// we are stateful
	T *cell_heap;
	int last_occupied_cell; // performance improvement
	bool *cell_map;

	void allocate_cell_map()
	{
		cell_map = static_cast<bool*>(malloc(sz * sizeof(bool)));
		if ( nullptr == cell_map )
		{
			throw std::bad_alloc();
		}
	}

	heap_allocator()
	{
		cell_heap = static_cast<T*>(malloc(sz * sizeof(T)));
		if ( nullptr == cell_heap )
		{
			throw std::bad_alloc();
		}
		allocate_cell_map();
		reset();
	}

	template <class U> heap_allocator(const heap_allocator <U, sz>&)
	{
		cell_heap = static_cast<U*>(malloc(sz * sizeof(U)));
		if ( nullptr == cell_heap )
		{
			throw std::bad_alloc();
		}
		allocate_cell_map();
		reset();
	}

	void reset()
	{
		for (int ix =0; ix < sz; ++ix) 
		{
			cell_map[ix] = false;
		}
		last_occupied_cell = -1;		
	}

	template< class U >
	struct rebind
	{
		typedef heap_allocator<U, sz> other;
	};

	~heap_allocator()
	{
		free(cell_heap);
		free(cell_map);
	}

	int find_free_cell()
	{
		int cell_ix = 0;
		if ( last_occupied_cell >= 0 )
		{
			cell_ix = last_occupied_cell;
		}
		while( (cell_ix < sz) and (cell_map[cell_ix]) ) cell_ix++;
		if ( cell_ix >= sz ) return -1;
		return cell_ix;
	}

	void check_in(int cell)
	{
		cell_map[cell] = true;
		last_occupied_cell = cell; // always append 
	}

	void check_out(int cell)
	{
		cell_map[cell] = false;
		if ( cell < last_occupied_cell )
		{
			last_occupied_cell = cell - 1; // could be -1
		}
	}

	T* allocate(int n)
	{
		if ( n > 1 )
		{
			cout << "ERROR allocate: n not supportted !\n";
			throw std::bad_alloc();
		}
		int ix = find_free_cell();
		if (-1 == ix)
		{
			cout << "ERROR allocate return null !!!!!!!!\n";
			throw std::bad_alloc();
		}
		check_in(ix);
		return &(cell_heap[ix]);
	}

	void deallocate(T* p, std::size_t n)
	{
		if ( n > 1 )
		{
			cout << "ERROR deallocate: n not supportted !\n";
		}
		int offset = static_cast<int>(p - cell_heap);
		assert((offset >= 0) and (offset < sz));
		check_out(offset);
	}

	template <class Up, class... Args>
	void construct(Up* p, Args&&... args) {
		::new ((void*)p) Up(std::forward<Args>(args)...);
	}

	void destroy(T* p)
	{
		p->~T();
	}

};

template <class T, std::size_t sz1, class U, std::size_t sz2>
constexpr bool operator== (const heap_allocator<T, sz1>& a1, const heap_allocator<U, sz2>& a2) noexcept
{
	return false;
}

template <class T, std::size_t sz1, class U, std::size_t sz2>
constexpr bool operator!= (const heap_allocator<T, sz1>&, const heap_allocator<U, sz2>&) noexcept
{
	return true;
}

#endif
