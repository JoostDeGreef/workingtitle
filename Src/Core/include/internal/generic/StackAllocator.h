#pragma once

#include <deque>
#include <iostream>
#include <forward_list>
#include <functional>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

// SIZE in bytes
template <size_t SIZE>
struct StackAllocatorBuffer
{
private:
    struct info
    {
        int prev;
        int next;
    };
public:
    static constexpr int blocks = SIZE/sizeof(info);
    StackAllocatorBuffer()
    {
        data[0].prev = blocks;
        data[0].next = blocks;
    }
    void* allocate(std::size_t n)
    {
        return allocate(n, std::allocator<char>());
    }
    template<typename ALLOCATOR>
    typename ALLOCATOR::pointer allocate(std::size_t n, ALLOCATOR & allocator)
    {
        using pointer = std::allocator_traits<ALLOCATOR>::pointer;

        int index = 0;
        int needed = (int)((n * sizeof(typename ALLOCATOR::value_type) + sizeof(info) - 1) / sizeof(info));
        while(index< blocks)
        {
            int nextIndex = data[index].next;
            if(nextIndex>0)
            {
                int gap = nextIndex - index - needed - 1;
                if(gap > 1)
                {
                    int newIndex = nextIndex - needed - 1;
                    data[index].next = newIndex;
                    data[newIndex].prev = index;
                    data[newIndex].next = -nextIndex;
                    if(blocks != nextIndex) data[nextIndex].prev = newIndex;
                    return (pointer)&data[newIndex+1];
                }
                else if(gap == 0 || gap == 1)
                {
                    data[index].next = -nextIndex;
                    return (pointer)&data[index+1];
                }
            }            
            index = abs(nextIndex);
        }
        return allocator.allocate(n);
    }
    void deallocate(void *p, std::size_t n)
    {
        deallocate((char *)p, n, std::allocator<char>());
    }
    template<typename ALLOCATOR>
    void deallocate(typename ALLOCATOR::pointer p, std::size_t n, ALLOCATOR & allocator)
    {
        int index = (int)(((size_t)((info*)p - 1) - (size_t)&data[0]) / sizeof(info));
        if( (index >= 0) && (index < blocks))
        {
            data[index].next = -data[index].next;
            if (index > 0 && data[data[index].prev].next>0)
            {
                index = data[index].prev;
            }
            int start = index;
            while (data[index].next > 0 && index != blocks)
            {
                index = data[index].next;
            }
            data[start].next = index;
            if (index != blocks) data[index].prev = start;
        }
        else
        {
            allocator.deallocate(p, n);
        }
    }
    // debug/test functionality
    // this lists all the block indices.
    std::vector<int> getBlocks() const
    {
        std::vector<int> res(1, 0);
        int index = 0;
        do
        {
            res.emplace_back(data[index].next);
            index = abs(data[index].next);
        }
        while(index< blocks);
        return res;
    }
private:
    info data[blocks];
};

template <class T, class BUFFER>
class StackAllocator
{
public:
    using value_type         = T;
    using pointer            = value_type*;
    using const_pointer      = typename std::pointer_traits<pointer>::template rebind<value_type const>;
    using void_pointer       = typename std::pointer_traits<pointer>::template rebind<void>;
    using const_void_pointer = typename std::pointer_traits<pointer>::template rebind<const void>;

    using difference_type    = typename std::pointer_traits<pointer>::difference_type;
    using size_type          = std::make_unsigned_t<difference_type>;

    template <class U, class B> struct rebind {typedef StackAllocator<U, B> other;};

    template <class U, class B>
    friend class StackAllocator;

    StackAllocator(BUFFER & buffer) noexcept 
        : m_buffer(buffer)
    {} 
    
    template <class U, class B> 
    StackAllocator(StackAllocator<U, B> const& other) noexcept 
        : m_buffer(other.m_buffer)
    {} 

    value_type* allocate(std::size_t n)
    {
        return m_buffer.allocate(n, m_allocator);
    }

    void deallocate(value_type* p, std::size_t n) noexcept  
    {
        m_buffer.deallocate(p, n, m_allocator);  
    }

    value_type* allocate(std::size_t n, const_void_pointer)
    {
        return allocate(n);
    }

    template <class U, class ...Args>
    void construct(U* p, Args&& ...args)
    {
        m_allocator.construct(p, std::forward<Args>(args)...);
    }

    template <class U>
    void destroy(U* p) noexcept
    {
        m_allocator.destroy(p);
    }

    std::size_t max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max();
    }

    StackAllocator select_on_container_copy_construction() const
    {
        return *this;
    }

    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap            = std::false_type;
    using is_always_equal                        = std::is_empty<StackAllocator>;

private:
    BUFFER & m_buffer;
    std::allocator<T> m_allocator;
};

template <class T1, class B1, class T2, class B2>
bool operator == (const StackAllocator<T1, B1>& lhs, const StackAllocator<T2, B2>& rhs) noexcept
{
    return &lhs.m_buffer == &rhs.m_buffer;
}

template <class T1, class B1, class T2, class B2>
bool operator != (const StackAllocator<T1, B1>& lhs, const StackAllocator<T2, B2>& rhs) noexcept
{
    return !(lhs == rhs);
}    


//
// Usage:
//
// StackMemory<100> sm;
// auto v1 = sm.vector<int>();
// auto v2 = sm.vector<double>();
//
template<size_t SIZE = 8192>
struct StackMemory
{
    StackMemory()
    {}

    // read the memory requirements for std::deque and set SIZE accordingly!
    template<class T, typename ...Params>
    std::deque<T,StackAllocator<T,StackAllocatorBuffer<SIZE>>> deque(Params&&... params)
    {
        return std::deque<T,StackAllocator<T,StackAllocatorBuffer<SIZE>>>(std::forward<Params>(params)..., (StackAllocator<T,StackAllocatorBuffer<SIZE>>(buffer)));
    }  

    template<class T, typename ...Params>
    std::forward_list<T,StackAllocator<T,StackAllocatorBuffer<SIZE>>> forward_list(Params&&... params)
    {
        return std::forward_list<T,StackAllocator<T,StackAllocatorBuffer<SIZE>>>(std::forward<Params>(params)..., (StackAllocator<T,StackAllocatorBuffer<SIZE>>(buffer)));
    }  

    template<class T, typename ...Params>
    std::list<T,StackAllocator<T,StackAllocatorBuffer<SIZE>>> list(Params&&... params)
    {
        return std::list<T,StackAllocator<T,StackAllocatorBuffer<SIZE>>>(std::forward<Params>(params)..., (StackAllocator<T,StackAllocatorBuffer<SIZE>>(buffer)));
    }  

    template<class Key, class T, class Compare = std::less<Key>, typename ...Params>
    std::map<Key,T,Compare,StackAllocator<std::pair<const Key, T>,StackAllocatorBuffer<SIZE>>> map(Params&&... params)
    {
        return std::map<Key,T,Compare,StackAllocator<std::pair<const Key, T>,StackAllocatorBuffer<SIZE>>>(std::forward<Params>(params)..., (StackAllocator<std::pair<const Key, T>,StackAllocatorBuffer<SIZE>>(buffer)));
    }  

    template<class Key, class T, class Compare = std::less<Key>, typename ...Params>
    std::multimap<Key,T,Compare,StackAllocator<std::pair<const Key, T>,StackAllocatorBuffer<SIZE>>> multimap(Params&&... params)
    {
        return std::multimap<Key,T,Compare,StackAllocator<std::pair<const Key, T>,StackAllocatorBuffer<SIZE>>>(std::forward<Params>(params)..., (StackAllocator<std::pair<const Key, T>,StackAllocatorBuffer<SIZE>>(buffer)));
    }  

    template<class Key, class Compare = std::less<Key>, typename ...Params>
    std::multiset<Key,Compare,StackAllocator<Key,StackAllocatorBuffer<SIZE>>> multiset(Params&&... params)
    {
        return std::multiset<Key,Compare,StackAllocator<Key,StackAllocatorBuffer<SIZE>>>(std::forward<Params>(params)..., (StackAllocator<Key,StackAllocatorBuffer<SIZE>>(buffer)));
    }  

    template<class Key, class Compare = std::less<Key>, typename ...Params>
    std::set<Key,Compare,StackAllocator<Key,StackAllocatorBuffer<SIZE>>> set(Params&&... params)
    {
        return std::set<Key,Compare,StackAllocator<Key,StackAllocatorBuffer<SIZE>>>(std::forward<Params>(params)..., (StackAllocator<Key,StackAllocatorBuffer<SIZE>>(buffer)));
    }  

    template<class T, typename ...Params>
    std::vector<T,StackAllocator<T,StackAllocatorBuffer<SIZE>>> vector(Params&&... params)
    {
        return std::vector<T,StackAllocator<T,StackAllocatorBuffer<SIZE>>>(std::forward<Params>(params)..., (StackAllocator<T,StackAllocatorBuffer<SIZE>>(buffer)));
    }  

protected:
    StackAllocatorBuffer<SIZE> buffer;

};
