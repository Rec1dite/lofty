#include <iostream>
#include "iter.h"

using namespace lofty;

//---------- Forward declarations ----------//
template<class> class VectorIterator;
class VectorForwardPolicy;
class Vector;

//========== Concrete Policies ==========//
// Use GenScatterHierarchy to build a unified policy from a set of other policies
// E.g. First policy defines getNext(), 2nd defines hasMore(), etc.

// TODO: VectorIterator should take a TypeList of config types E.g. ValType, StepType, EndType
template<class Policy>
class VectorIterator : public Iterator<VectorIterator, Vector, int, Policy> {
    private:
        int index;

    public:
        // TODO: Work out how to get rid of this (base constructor does everything anyway)
        using Base = typename Iterator<VectorIterator, Vector, int, Policy>::Base;
        VectorIterator(Vector* vec) : Base(vec) {}

        // Only function that each concrete iterator is required to implement
        int getCurrent() {
            return this->structure->at(index);
        }

    friend Policy;
};

class Vector : public Structure<VectorIterator, Vector, int> {
    int* data;
    int size;

    public:
        Vector(int* data, int size) {
            this->size = size;
            this->data = new int[size];
            for (int i = 0; i < size; i++) { this->data[i] = data[i]; }
        }

        int at(int index) {
            return this->data[index];
        }

        //========== Policies ==========//
        class ForwardPolicy {
            private:
                using This = ForwardPolicy;
            
            protected:
                // If the policy needs to create state on the iterator, it can do so here

            public:
                // Required for every policy
                static void _goToStart(VectorIterator<This>* iterator, Vector* vec) {
                    iterator->index = 0;
                };

                // Required for every policy
                static void _step(VectorIterator<This>* iterator, Vector* vec) {
                    if (iterator->index < vec->size) { iterator->index++; }
                };

                // Required for every policy
                static bool _hasReachedEnd(VectorIterator<This>* iterator, Vector* vec) {
                    return iterator->index >= vec->size;
                };
        };

        // Policy to loop from end of vector to start
        class BackwardsPolicy {
            private:
                using This = BackwardsPolicy;

            public:
                // Required for every policy
                static void _goToStart(VectorIterator<This>* iterator, Vector* vec) {
                    iterator->index = vec->size - 1;
                };

                // Required for every policy
                static void _step(VectorIterator<This>* iterator, Vector* vec) {
                    if (iterator->index > 0) { iterator->index--; }
                };

                // Required for every policy
                static bool _hasReachedEnd(VectorIterator<This>* iterator, Vector* vec) {
                    return iterator->index <= 0;
                };
        };

    // Alternatively, if policies are declared externally (not nested), they must be declared as friends:
    // friend class MyPolicy1;
    // friend class MyPolicy2;
};

//========== Usage ==========//

int main() {
    int* data = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    Vector* vec1 = new Vector(data, 10);

    auto iterF = vec1->createIterator<Vector::ForwardPolicy>();
    auto iterB = vec1->createIterator<Vector::BackwardsPolicy>();

    while(iterF->hasMore()) {
        std::cout << iterF->getCurrent() << std::endl;
        iterF->getNext();
    }
    std::cout << "====================" << std::endl;
    while(iterB->hasMore()) {
        std::cout << iterB->getCurrent() << std::endl;
        iterB->getNext();
    }

    return 0;
}
