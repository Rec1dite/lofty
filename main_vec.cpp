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
                    if (!_hasReachedEnd(iterator, vec)) { iterator->index++; }
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
                    if (!_hasReachedEnd(iterator, vec)) { iterator->index--; }
                };

                // Required for every policy
                static bool _hasReachedEnd(VectorIterator<This>* iterator, Vector* vec) {
                    return iterator->index < 0;
                };
        };

        template<unsigned int NumBounces, int Step>
        class PingPongPolicy {
            private:
                using This = PingPongPolicy<NumBounces, Step>;
            
            protected:
                int bounces = 0;
            
            public:
                // Required for every policy
                static void _goToStart(VectorIterator<This>* iterator, Vector* vec) {
                    iterator->bounces = 0;
                    iterator->index = (Step > 0) ? 0 : (vec->size - 1);
                };

                // Required for every policy
                static void _step(VectorIterator<This>* iterator, Vector* vec) {
                    if (_hasReachedEnd(iterator, vec)) { return; }

                    iterator->index += (iterator->bounces % 2 == 0) ? Step : -Step;

                    if (iterator->index < 0 || iterator->index >= vec->size) { iterator->bounces++; } // bounce

                    iterator->index = std::max(0, std::min(iterator->index, vec->size - 1)); // clamp
                };

                // Required for every policy
                static bool _hasReachedEnd(VectorIterator<This>* iterator, Vector* vec) {
                    return iterator->bounces >= NumBounces;
                };
        };
            
        template<class MonoidOperator>
        class FoldPolicy {
            private:
                using This = FoldPolicy<MonoidOperator>;
                using ValueType = typename MonoidOperator::ValueType;

            protected:
                ValueType acc;

            public:
                ValueType getAcc() { return acc; }

                // Required for every policy
                static void _goToStart(VectorIterator<This>* iterator, Vector* vec) {
                    iterator->acc = MonoidOperator::id;
                    iterator->index = 0;
                };

                // Required for every policy
                static void _step(VectorIterator<This>* iterator, Vector* vec) {
                    iterator->acc = MonoidOperator::op(iterator->acc, iterator->getCurrent());
                    iterator->index++;
                };

                // Required for every policy
                static bool _hasReachedEnd(VectorIterator<This>* iterator, Vector* vec) {
                    return iterator->index >= vec->size;
                };
        };

        //----- Monoidal Operators -----//
        class AddMonoid {
            public:
                using ValueType = int;

                static const int id = 0;
                static int op(int a, int b) { return a + b; }
        };

        class ConcatMonoid {
            public:
                using ValueType = std::string;

                const static std::string id;
                static std::string op(std::string a, int b) {
                    return a + std::to_string(b);
                };
        };

    // Alternatively, if policies are declared externally (not nested), they must be declared as friends:
    // friend class MyPolicy1;
    // friend class MyPolicy2;
};
const std::string Vector::ConcatMonoid::id = "";

//========== Usage ==========//

int main() {
    int* data = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    Vector* vec1 = new Vector(data, 10);

    auto iterF = vec1->createIterator<Vector::ForwardPolicy>();
    auto iterB = vec1->createIterator<Vector::BackwardsPolicy>();

    std::cout << "\033[96m========== Forward Policy ==========\033[0m" << std::endl;
    while(iterF()) {
        // std::cout << iterF++ << std::endl;
        std::cout << *(iterF) << "  ";
        // iterF->getNext();       
         iterF++;
    }

    std::cout << std::endl << "\033[96m========== Backwards Policy ==========\033[0m" << std::endl;
    while(iterB.hasMore()) {
        // std::cout << iterB++ << std::endl;
        std::cout << iterB.getCurrent() << "  ";
        iterB.getNext();
    }

    std::cout << std::endl << "\033[96m========== Ping Pong Policy ==========\033[0m" << std::endl;
    for(auto iterJ = vec1->createIterator<Vector::PingPongPolicy<3, 2>>(); iterJ.hasMore(); iterJ.getNext()) {
        std::cout << iterJ.getCurrent() << "  ";
    }

    std::cout << std::endl << "\033[96m========== Implicit Folding Policy ==========\033[0m" << std::endl;
    std::cout << std::endl << "\033[96mAddition Monoid:\033[0m" << std::endl;
    auto iterFoldAdd = vec1->createIterator<Vector::FoldPolicy<Vector::AddMonoid>>();

    while (iterFoldAdd.hasMore()) {
        std::cout << iterFoldAdd.getAcc() << "  ";
        iterFoldAdd.getNext();
    }
    std::cout << "\033[95mResult: " << iterFoldAdd.getAcc() << "\033[0m" << std::endl;

    std::cout << std::endl << "\033[96mConcatenation Monoid:\033[0m" << std::endl;
    auto iterFoldConcat = vec1->createIterator<Vector::FoldPolicy<Vector::ConcatMonoid>>();
    while (iterFoldConcat.hasMore()) {
        std::cout << iterFoldConcat.getAcc() << "  ";
        iterFoldConcat.getNext();
    }
    std::cout << "\033[95mResult: " << iterFoldConcat.getAcc() << "\033[0m" << std::endl;

    return 0;
}
