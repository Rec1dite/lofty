// #include <algorithm>
#include <iostream>
// #include <vector>
// #include <iterator>
#include "iter.h"

using namespace lofty;
using namespace std;

// template<long FROM, long TO>
class Range {
    public:
        class NullContainer;
        template <class Policy>
        class iterator : public Iterator<iterator, NullContainer, long, Policy> {
            public:
                using Base = typename Iterator<iterator, NullContainer, long, Policy>::Base;
                iterator(NullContainer* container) : Base(container) {}

                long getCurrent() {
                    return this->num;
                }

            friend Policy;
        };

        class NullContainer : public Structure<iterator, NullContainer, long> {
            public:
                //========== Policies ==========//
                class ForwardPolicy {
                    private:
                        using This = ForwardPolicy;

                    public:
                        long num = 0;

                        static void _goToStart(iterator<This>* iterator, NullContainer* container) {
                            iterator->num = 0;
                        }

                        static void _step(iterator<This>* iterator, NullContainer* container) {
                            if (!_hasReachedEnd(iterator, container)) { iterator->num++; }
                        }

                        static bool _hasReachedEnd(iterator<This>* iterator, NullContainer* container) {
                            return iterator->num >= 5;
                        }
                };
        };

        NullContainer* container;

        Range() { this->container = new NullContainer(); }

        iterator<NullContainer::ForwardPolicy>* begin()  {
            return container->createIterator<NullContainer::ForwardPolicy>();
        }
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
//         iterator& operator++() { num = TO >= FROM ? num + 1: num - 1; return *this; }
//         iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }
//         bool operator==(iterator other) const { return num == other.num; }
//         bool operator!=(iterator other) const { return !(*this == other); }
//         reference operator*() const { return num; }
//     };
//     iterator begin() { return iterator(FROM); }
//     iterator end() { return iterator(TO >= FROM? TO + 1 : TO - 1); }
// };

int main()
{
    auto range = Range();
    auto itr = range.begin();
    // std::find requires an input iterator
    // auto itr = std::find(range.begin(), range.end(), 18);
    itr->getNext();
    std::cout << itr->getCurrent() << '\n'; // 18

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
