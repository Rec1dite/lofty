#include <iostream>

#include "iter.h"

using namespace lofty;

//---------- Forward declarations ----------//
template <class>
class RangeIterator;
template <long, long>
class RangeForwardPolicy;
class NullStructure;

using NullRange = lofty::NullStructure<RangeIterator, int>;

template <class Policy>
class RangeIterator : public Iterator<RangeIterator, int, Policy> {
   private:
    int pos = 0;

   public:
    int getCurrent() { return pos; }

    friend Policy;
};

template <long FROM, long TO>
class RangeForwardPolicy {
   private:
    using This = RangeForwardPolicy<FROM, TO>;

   protected:
    long from = FROM;
    long to = TO;

   public:
    static void _goToStart(RangeIterator<This>* itr) {
        itr->pos = FROM;
    };

    static void _step(RangeIterator<This>* itr) {
        if (!_hasReachedEnd(itr, _)) {
            itr->pos++;
        }
    };

    static bool _hasReachedEnd(RangeIterator<This>* itr,) {
        return itr->pos >= itr->to;
    };
};

template <long FROM, long TO>
auto range() {
    return Structure<RangeIterator, NullRange, int>()
        .createIterator<RangeForwardPolicy<FROM, TO>>();
}

//========== Usage ==========//

int main() {
    auto iterF = range<10, 15>();

    std::cout << "\033[96m========== Forward Policy ==========\033[0m"
              << std::endl;
    while (iterF()) {
        std::cout << iterF++ << "  ";
    }
    std::cout << std::endl;

    // RangeWrapper<15, 25> range2;

    // auto iterF2 = range2.begin();

    // std::cout << "\033[96m========== Forward Policy ==========\033[0m" <<
    // std::endl;

    // while(iterF2()) {
    //     std::cout << iterF2++ << "  ";
    // }

    // std::cout << std::endl;

    return 0;
}
