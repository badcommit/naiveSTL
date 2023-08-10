//
// Created by Shawn Zhang on 8/8/23.
//

#ifndef NAIVESTL_ENUMERATE_H
#define NAIVESTL_ENUMERATE_H

#include <utility>
#include "naiveSTL/type_traits.h"
#include "iterator.h"
#include <memory>

namespace NaiveSTL {
    template<class T>
    struct [[maybe_unused]] MakeConst {
        using type = const T;
    };

    template<class T>
    struct MakeConst<T&>{
        using type = const T&;
    };

    template<class T>
    struct MakeConst<T*>{
        using type = const T*;
    };

    template<class Iterator>
    class Enumerator{
    public:
        constexpr explicit Enumerator(Iterator it): it_(std::move(it)){}

        class Proxy {
            using difference_type = ssize_t;
            using value_type = typename iterator_traits<Iterator>::value_type;
            using reference = typename iterator_traits<Iterator>::reference;
            using pointer = typename iterator_traits<Iterator>::pointer;
            using iterator_category = input_iterator_tag;

            inline constexpr explicit Proxy(const Enumerator& e): index(e.idx_), element(*e.it_){}

            inline constexpr reference operator*() { return element; }
            inline constexpr pointer operator->() { return std::addressof(element); }

            inline constexpr typename MakeConst<reference>::type operator *() const { return element; }

            inline constexpr typename MakeConst<pointer>::type operator ->() const { return std::addressof(element); }

        private:
            const size_t index;
            reference element;
        };

    public:
        inline constexpr Proxy operator*() const { return Proxy(*this); }

        inline constexpr Enumerator& operator++() {
            ++it_;
            ++idx_;
            return *this;
        }

        inline constexpr Enumerator operator++(int) {
            Enumerator tmp = *this;
            ++*this;
            return tmp;
        }

        inline constexpr bool operator==(const Enumerator& rhs) const { return it_ == rhs.it_; }

        inline constexpr bool operator!=(const Enumerator& rhs) const { return it_ != rhs.it_; }

        inline constexpr size_t index() const { return idx_; }

    private:
        Iterator it_;
        size_t idx_ = 0;
    };

    template<class Range>
    class RangeEnumerator{
        Range r_;
        using BeginIterator = decltype(std::begin(r_));
        using EndIterator = decltype(std::end(r_));

    public:
        constexpr explicit RangeEnumerator(Range&& r): r_(std::move(r)){}

        inline constexpr Enumerator<BeginIterator> begin() { return Enumerator<BeginIterator>(std::begin(r_)); }

        inline constexpr Enumerator<EndIterator> end() { return Enumerator<EndIterator>(std::end(r_)); }
    };

    template<class Range>
    constexpr RangeEnumerator<Range> enumerate(Range&& r){
        return RangeEnumerator<Range>(std::forward<Range>(r));
    }
}



#endif //NAIVESTL_ENUMERATE_H
