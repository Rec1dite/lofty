#include <iostream>
#include "iter.h"

using namespace lofty;

//---------- Forward declarations ----------//
template<class> class VectorIterator;
class VectorForwardPolicy;
class Range;

//========== Concrete Policies ==========//

template<class Policy>
class VectorIterator : public Iterator<VectorIterator, Range, int, Policy> {
    private:
        int pos = 0;

    public:
        // TODO: Work out how to get rid of this (base constructor does everything anyway)
        using Base = typename Iterator<VectorIterator, Range, int, Policy>::Base;
        VectorIterator(Range* vec) : Base(vec) {}

        int getCurrent() { return pos; }

    friend Policy;
};

class Range : public Structure<VectorIterator, Range, int> {
    int from;
    int to;

    public:
        Range(int from, int to) : from(from), to(to) {}

        //========== Policies ==========//
        class ForwardPolicy {
            private:
                using This = ForwardPolicy;
            
            protected:
                // If the policy needs to create state on the iterator, it can do so here

            public:
                // Required for every policy
                static void _goToStart(VectorIterator<This>* iterator, Range* range) {
                    iterator->pos = range->from;
                };

                // Required for every policy
                static void _step(VectorIterator<This>* iterator, Range* range) {
                    if (!_hasReachedEnd(iterator, range)) { iterator->pos++; }
                };

                // Required for every policy
                static bool _hasReachedEnd(VectorIterator<This>* iterator, Range* range) {
                    return iterator->pos >= range->to;
                };
        };

    // Alternatively, if policies are declared externally (not nested), they must be declared as friends:
    // friend class MyPolicy1;
    // friend class MyPolicy2;
};

template<long FROM, long TO>
class RangeWrapper {
    public:
        Range* range;

        RangeWrapper() {
            range = new Range(FROM, TO);
        }

        template<class Policy>
        VectorIterator<Policy> begin() {
            return VectorIterator<Policy>(range);
        }
};

//========== Usage ==========//

int main() {
    Range* range = new Range(10, 25);

    auto iterF = range->createIterator<Range::ForwardPolicy>();

    std::cout << "\033[96m========== Forward Policy ==========\033[0m" << std::endl;
    while(iterF()) {
        std::cout << iterF++ << "  ";
    }
    std::cout << std::endl;


    RangeWrapper<15, 25> range2;

    auto iterF2 = range2.begin<Range::ForwardPolicy>();

    std::cout << "\033[96m========== Forward Policy ==========\033[0m" << std::endl;

    while(iterF2()) {
        std::cout << iterF2++ << "  ";
    }

    std::cout << std::endl;

    return 0;
}
