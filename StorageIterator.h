#pragma once

#include <iterator>


namespace ecs
{
    template<typename Container>
    struct SparseSetIterator
    {
        using value_type = typename Container::value_type;
        using pointer = typename Container::const_pointer;
        using reference = typename Container::const_reference;
        using difference_type = typename Container::difference_type;
        using iterator_category = std::forward_iterator_tag;

        SparseSetIterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        SparseSetIterator& operator++() { m_ptr++; return *this; }
        SparseSetIterator operator++(int) { SparseSetIterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const SparseSetIterator& a, const SparseSetIterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const SparseSetIterator& a, const SparseSetIterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer m_ptr;
    };
}
