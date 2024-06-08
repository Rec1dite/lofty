// #include <algorithm>
// #include <vector>
// #include <iterator>
#include <iostream>

#include "iter.h"

using namespace lofty;

template <long FROM, long TO>
class Range {
    template <class Policy>
    class iterator : public Iterator<long, Policy, iterator> {
       public:
        iterator(long num) : Iterator<long, Policy, iterator>() {
            this->num = num;
        }

        long getCurrent() { return this->num; }

        long operator*() { return getCurrent(); };
    };

    //========== Policies ==========//
    struct Forward {
        long num;

        static void _goToStart(iterator<Forward>* iterator) {
            iterator->num = FROM;
        }

        static void _step(iterator<Forward>* iterator) {
            if (!_hasReachedEnd(iterator)) iterator->num++;
        }

        static bool _hasReachedEnd(iterator<Forward>* iterator) {
            return iterator->num >= TO;
        }
    };

   public:
    auto begin() { return iterator<Forward>(FROM); }

    auto end() { return iterator<Forward>(TO); }
};

// template<long FROM, long TO>
// class Range1
// {
//     public:
//     // member typedefs provided through inheriting from std::iterator
//     class iterator : public std::iterator<
//                                 std::input_iterator_tag, // iterator_category
//                                 long,                    // value_type
//                                 long,                    // difference_type
//                                 const long*,             // pointer
//                                 long                     // reference
//                             > {
//         long num = FROM;
//     public:
//         explicit iterator(long _num = 0) : num(_num) {}
//         iterator& operator++() { num = TO >= FROM ? num + 1: num - 1; return
//         *this; } iterator operator++(int) { iterator retval = *this;
//         ++(*this); return retval; } bool operator==(iterator other) const {
//         return num == other.num; } bool operator!=(iterator other) const {
//         return !(*this == other); } reference operator*() const { return num;
//         }
//     };
//     iterator begin() { return iterator(FROM); }
//     iterator end() { return iterator(TO >= FROM? TO + 1 : TO - 1); }
// };

int main() {
    auto range = Range<0, 10>();
    // auto range = Range();
    auto itr = range.begin();
    // std::find requires an input iterator
    // auto itr = std::find(range.begin(), range.end(), 18);
    itr.getNext();
    std::cout << *itr << std::endl;            // 18
    std::cout << *range.begin() << std::endl;  // 18
    std::cout << *range.end() << std::endl;    // 18

    // Range::iterator also satisfies range-based for requirements
    // for (long l : Range<3, 5>())
    // std::cout << l << ' '; // 3 4 5
    // std::cout << '\n';

    // std::vector<int> ar = { 1, 2, 3, 4 };
    // std::vector<int>::iterator ptr = ++(++ar.begin());

    // for (auto x = ptr; ptr >= ar.begin(); ptr--) {
    //     std::cout << *ptr << " ";
    // }
}


int main() {
    auto range = Range<0, 10>();
    std::cout << *range.begin() << std::endl; // 0
    std::cout << *range.end() << std::endl;   // 10
}
