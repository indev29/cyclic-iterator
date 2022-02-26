
//          Copyright Maxim Kolesnikov 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef IDV_CYCLIC_ITERATOR_H
#define IDV_CYCLIC_ITERATOR_H

#include <cmath>
#include <iterator>
#include <type_traits>
#include <utility>

#if __cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L)
#define IDV_CYCLIC_ITERATOR_CONSTEXPR constexpr
#else
#define IDV_CYCLIC_ITERATOR_CONSTEXPR
#endif

namespace idv {

template<typename It>
class CyclicIterator
{
private:
    using base_traits = std::iterator_traits<It>;
    using base_category = typename base_traits::iterator_category;

    static_assert(
        std::is_base_of<std::forward_iterator_tag, base_category>::value,
        "Base iterator must have a category of at least ForwardIterator"
    );

public:
    using iterator_type = It;

    using difference_type = typename base_traits::difference_type;
    using value_type = typename base_traits::value_type;
    using pointer = typename base_traits::pointer;
    using reference = typename base_traits::reference;
    using iterator_category = typename base_traits::iterator_category;

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    CyclicIterator(It begin, It end)
        :begin(begin), cur(begin), end(end)
    {}

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    iterator_type base() const
    {
        return cur;
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    reference operator*() const
    {
        return *cur;
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    pointer operator->() const
    {
        return cur.operator->();
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    CyclicIterator & operator++()
    {
        ++cur;
        if (cur == end)
        {
            cur = begin;
        }

        return *this;
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    CyclicIterator operator++(int)
    {
        auto copy = *this;
        operator++();
        return copy;
    }

    // Since multipass guarantee of ForwardIterator requires
    // consistent equality results between increments,
    // its equality operator should also compare begin and end
    //
    // The exact conflicting part of the guarantee is:
    //   a == b implies ++a == ++b
    // which is not the case if begin or end differ
    IDV_CYCLIC_ITERATOR_CONSTEXPR
    bool operator==(const CyclicIterator other) const
    {
        return (begin == other.begin &&
                cur == other.cur &&
                end == other.end);
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    bool operator!=(const CyclicIterator other) const
    {
        return !(operator==(other));
    }

    // BidirectionalIterator
    //

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    CyclicIterator & operator--()
    {
        if (cur == begin)
        {
            cur = end;
        }
        --cur;

        return *this;
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    CyclicIterator operator--(int)
    {
        auto copy = *this;
        operator--();
        return copy;
    }

    // RandomAccessIterator
    //
    
    IDV_CYCLIC_ITERATOR_CONSTEXPR
    reference operator[](difference_type n) const
    {
        return cur[n];
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    CyclicIterator & operator+=(difference_type n)
    {
        advance(n);
        return *this;
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    CyclicIterator & operator-=(difference_type n)
    {
        advance(-n);
        return *this;
    }

    friend IDV_CYCLIC_ITERATOR_CONSTEXPR
    CyclicIterator operator+(CyclicIterator it, difference_type n)
    {
        return it += n;
    }

    friend IDV_CYCLIC_ITERATOR_CONSTEXPR
    CyclicIterator operator+(difference_type n, CyclicIterator it)
    {
        return operator+(it, n);
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    CyclicIterator operator-(difference_type n) const
    {
        auto copy = *this;
        return copy -= n;
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    difference_type operator-(const CyclicIterator & other) const
    {
        return (cur - other.cur);
    }

    // FIXME: total ordering relation requires
    // exactly one of the following expressions to be true:
    //   a < b or b < a or a == b
    // none of them will be true if
    // a.cur == b.cur and begin or end differ (see operator==)
    IDV_CYCLIC_ITERATOR_CONSTEXPR
    bool operator<(const CyclicIterator & other) const
    {
        return (cur < other.cur);
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    bool operator>(const CyclicIterator & other) const
    {
        return (cur > other.cur);
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    bool operator<=(const CyclicIterator & other) const
    {
        return (cur <= other.cur);
    }

    IDV_CYCLIC_ITERATOR_CONSTEXPR
    bool operator>=(const CyclicIterator & other) const
    {
        return (cur >= other.cur);
    }

private:
    IDV_CYCLIC_ITERATOR_CONSTEXPR
    void advance(difference_type n)
    {
        if (n >= 0)
        {
            auto tail = end - cur;
            if (n < tail)
            {
                cur += n;
            }
            else
            {
                auto size = end - begin;
                if (!size)
                {
                    return;
                }

                cur = begin + (n - tail) % size;
            }
        }
        else //if (n < 0)
        {
            auto tail = (begin - cur) - 1;
            if (n > tail)
            {
                cur += n;
            }
            else
            {
                auto size = end - begin;
                if (size <= 1)
                {
                    return;
                }

                cur = end - 1 - (tail - n) % (size);
            }
        }
    }

    It begin;
    It cur;
    It end;
};

// Pre-C++17 helper
template<typename It>
IDV_CYCLIC_ITERATOR_CONSTEXPR
CyclicIterator<It> make_cyclic_iterator(It begin, It end)
{
    return CyclicIterator<It>(begin, end);
}

}

#undef IDV_CYCLIC_ITERATOR_CONSTEXPR

#endif
