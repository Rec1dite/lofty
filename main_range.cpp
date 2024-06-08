#include <iostream>

#include "iter.h"
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

        long operator*() { return getCurrent(); };

        // Preincrement
        auto operator++() {
            Policy::_step(this);
            return *this;
        }

        // Postincrement
        auto operator++(int) {
            auto res = *this;
            Policy::_step(this);
            return res;
        }

        bool operator==(const iterator<Policy>& other) const {
            return this->num == other.num;
        }

        bool operator!=(const iterator<Policy>& other) const {
            return this->num != other.num;
        }

        bool operator>=(const iterator<Policy>& other) const {
            return this->num >= other.num;
        }
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
    auto begin() { return iterator<Forward>(FROM); }

    auto end() { return iterator<Forward>(TO); }
};

int main() {
    heading("Normal Loop");

    auto range1 = Range<0, 10>();

    for (auto i = range1.begin(); i != range1.end(); ++i) {
        std::cout << *i << " ";
    }

    heading("Normal Loop With Step", 2);

    auto range2 = Range<10, 0, -2>();

    for (auto i = range2.begin(); i >= range2.end(); ++i) {
        std::cout << *i << " ";
    }

    heading("Foreach Loop", 2);

    for (auto i : Range<0, 10>()) {
        std::cout << i << " ";
    };

    std::cout << std::endl;
}
