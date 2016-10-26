#pragma once

#include <cstddef>

template <typename T, std::size_t Alignment>
class aligned_allocator
{
public:
    typedef T * pointer;
    typedef const T * const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;
    typedef std::size_t size_type;
    typedef ptrdiff_t difference_type;
public:
    value_type *address(value_type& r) const
    {
        return &r;
    }

    const value_type * address(const value_type& s) const
    {
        return &s;
    }

    std::size_t max_size() const
    {
        return (static_cast<std::size_t>(0) - static_cast<std::size_t>(1)) / sizeof(value_type);
    }

    template <typename U>
    struct rebind
    {
        typedef aligned_allocator<U, Alignment> other;
    };

    bool operator!=(const aligned_allocator& other) const
    {
        return !(*this == other);
    }

    void construct(value_type * const p, const value_type& t) const
    {
        void * const pv = static_cast<void *>(p);
        new (pv) value_type(t);
    }

    void destroy(value_type * const p) const
    {
        p->~T();
    }

    bool operator==(const aligned_allocator& other) const
    {
        return true;
    }

    aligned_allocator() { }

    aligned_allocator(const aligned_allocator&) { }

    template <typename U>
    aligned_allocator(const aligned_allocator<U, Alignment>&) { }

    ~aligned_allocator() { }

    value_type * allocate(const std::size_t n) const
    {
        if (n == 0)
            return nullptr;

        if (n > max_size())
            throw std::length_error(
                        "aligned_allocator<T>::allocate() - Integer overflow.");

        void * const ptr = _mm_malloc(n * sizeof(value_type), Alignment);

        if (ptr == nullptr)
            throw std::bad_alloc();

        return static_cast<value_type *>(ptr);
    }

    void deallocate(value_type * const p, const std::size_t /*n*/) const
    {
        _mm_free(p);
    }

    template <typename U>
    value_type * allocate(const std::size_t n, const U *) const
    {
        return allocate(n);
    }

private:
    aligned_allocator& operator=(const aligned_allocator&);
};
