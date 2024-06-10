#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "lofty.h"
#include "utils.h"

using namespace lofty;

template <long FROM, long TO, long STEP = 1>
class Range {
    //========== Internal Iterator  ==========//
    template <class Policy>
    class iterator : public Iterator<long, Policy, iterator> {
        public:
            iterator(long num) : Iterator<long, Policy, iterator>() {
                this->num = num;
            }

            long getCurrent() { return this->num; }

            #define OPERATOR(OP) \
                bool operator OP(const iterator<Policy>& other) const { \
                    return this->num OP other.num; \
                }

            OPERATOR(==)
            OPERATOR(!=)
            OPERATOR(<=)
            OPERATOR(>=)
            OPERATOR(<)
            OPERATOR(>)

            #undef OPERATOR
        };

    //========== Iterator Policies ==========//
    struct Forward {
        long num;

        static void _goToStart(iterator<Forward>* iterator) {
            iterator->num = FROM;
        }

        static void _step(iterator<Forward>* iterator) {
            iterator->num += STEP;
        }

        static bool _hasReachedEnd(iterator<Forward>* iterator) {
            return iterator->num <= TO;
        }
    };

    public:
        // To be compatible with std::iterator, a structure may
        // simply define the following two methods, as well as
        // comparison operators seen above.
        auto begin() const { return iterator<Forward>(FROM); }
        auto end() const { return iterator<Forward>(TO); }
};

int main() {
    auto range1 = Range<0, 10>();
    auto range2 = Range<10, 0, -2>();

    heading("Normal Loop");
    for (auto i = range1.begin(); i != range1.end(); ++i) {
        std::cout << *i << " ";
    }

    heading("Normal Loop With Step", 2);
    for (auto i = range2.begin(); i >= range2.end(); ++i) {
        std::cout << *i << " ";
    }

    heading("Foreach Loop", 2);
    for (long l : Range<-5, 8>()) {
        std::cout << l << " ";
    };

    heading("With std::find", 2);
    auto it = std::find(range1.begin(), range1.end(), 4);

    if (it != range1.end()) {
        std::cout << "\033[93mFound: " << *it << "\033[0m" << std::endl;
    } else {
        std::cout << "\033[95mNot found\033[0m" << std::endl;
    }

    std::cout << std::endl << std::endl;

    return 0;
}
