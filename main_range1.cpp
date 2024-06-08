#include <iostream>
#include "iter.h"

using namespace lofty;

//---------- Forward declarations ----------//
template<class> class RangeIterator;
template<long,long> class RangeForwardPolicy;
class NullStructure;

//========== Concrete Policies ==========//

using NullRange = lofty::NullStructure<RangeIterator, int>;

template<class Policy>
class RangeIterator : public Iterator<RangeIterator, NullRange, int, Policy> {
    private:
        int pos = 0;

    public:
        // TODO: Work out how to get rid of this (base constructor does everything anyway)
        using Base = typename Iterator<RangeIterator, NullRange, int, Policy>::Base;
        RangeIterator(NullRange* vec) : Base(vec) {}

        int getCurrent() { return pos; }

    friend Policy;
};

template<long FROM, long TO>
class RangeForwardPolicy {
    private:
        using This = RangeForwardPolicy<FROM, TO>;
    
    protected:
        long from = FROM;
        long to = TO;

    public:
        // Required for every policy
        static void _goToStart(RangeIterator<This>* iterator, NullRange* _) {
            iterator->pos = iterator->from;
        };

        // Required for every policy
        static void _step(RangeIterator<This>* iterator, NullRange* _) {
            if (!_hasReachedEnd(iterator, _)) { iterator->pos++; }
        };

        // Required for every policy
        static bool _hasReachedEnd(RangeIterator<This>* iterator, NullRange* _) {
            return iterator->pos >= iterator->to;
        };
};

template<long FROM, long TO>
auto range() {
    return Structure<RangeIterator, NullRange, int>().createIterator<RangeForwardPolicy<FROM, TO>>();
}

// template<long FROM, long TO>
// class RangeWrapper {
//     public:
//         NullStructure* range;

        // RangeWrapper() { range = new NullStructure(); }

        // RangeIterator<RangeForwardPolicy> begin() {
            // return RangeIterator<RangeForwardPolicy>(range);
        // }
// };

//========== Usage ==========//

int main() {

    auto iterF = range<10, 15>();

    std::cout << "\033[96m========== Forward Policy ==========\033[0m" << std::endl;
    while(iterF()) {
        std::cout << iterF++ << "  ";
    }
    std::cout << std::endl;

    // RangeWrapper<15, 25> range2;

    // auto iterF2 = range2.begin();

    // std::cout << "\033[96m========== Forward Policy ==========\033[0m" << std::endl;

    // while(iterF2()) {
    //     std::cout << iterF2++ << "  ";
    // }

    // std::cout << std::endl;

    return 0;
}
