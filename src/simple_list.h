#ifndef _SIMPLE_LIST_H_
#define _SIMPLE_LIST_H_

#include <memory>

/*
template <typename T>
struct list_element_t
{
    T data;
    list_element_t* next;
    list_element_t() : next(nullptr) {}
    list_element_t(const T &he) : data(he), next(nullptr) {}
};
*/
//template <typename T, class Allocator = std::allocator< list_element_t<T>> >
template <typename T, typename allocator_t >
class simple_list_t {
    struct list_element_t
    {
        T data;
        list_element_t* next;
        list_element_t() : next(nullptr) {}
        list_element_t(const T &he) : data(he), next(nullptr) {}
    };

public:
    simple_list_t()
    {
        reset();
    }

    void reset()
    {
        head = nullptr;
        tail = nullptr;
    }

    ~simple_list_t()
    {
        list_element_t *current = head; // use current as tmp ptr
        while (head != nullptr)
        {
            current = head;
            head = head->next;
            alloc.deallocate(current, 1);
        }
    }

    class iterator
    {
    private:
        list_element_t *ptr;
    public:
        iterator() : ptr(nullptr) { }

        iterator(list_element_t* p) : ptr(p) { }

        T& operator*()
        {
            return ptr->data;
        }

        // prefix
        iterator& operator ++ ()
        {
            ptr = ptr->next;
            return *this;
        }

        // postfix
        iterator operator ++ (int)
        {
            ptr = ptr->next;
            return *this;
        }        

        bool operator!=(const iterator & him)
        {
            return ptr != him.ptr;
        }

        bool operator==(const iterator & him)
        {
            return ptr == him.ptr;
        }

    };

    void append(const T& x)
    {
        list_element_t *new_item = alloc.allocate(1);
        alloc.construct(new_item, x);
        if (nullptr == head)
        {
            head = new_item;
            tail = new_item;
            return;
        }
        tail->next = new_item;
        tail = new_item;
    }
    
    iterator begin()
    {
        iterator it(head);
        return it;
    }
    
    iterator end()
    {
        iterator it(tail->next);
        return it;
    }

private:
    list_element_t *head, *tail;
    typename allocator_t::template rebind<list_element_t>::other alloc;
};

#endif // _SIMPLE_LIST_H_