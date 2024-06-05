#include <iostream>

// Steps for usage:
// 1) Create some kind of data structure that you want to iterate through. This data structure must inherit from Iterable<>
// 2) Create a generalized iterator class for that structure. This class must inherit from Iterator<>
// 3) Create one or more policies which define _how_ to iterate through the data structure. Each inherits from IteratorPolicy<>
// 4) Pass the policy into the Iterator<> to create a new Iterator<YourPolicy> class which can be used to iterate through the structure

namespace lofty {
    //========== Interfaces ==========//

    // Use policy to define how to iterate through a structure (to be converted to Visitor)
    // ValueType is the type of a single item within the Iterable (that can be pointed to)
    template<class ConcreteIterable, class ValueType>
    class IteratorPolicy {
        public:
            // Describes how to move from one item in the iterable to the next
            static virtual void step(ConcreteIterable) = 0; // TODO: Make this a Visitor
            // Determines whether we have reached the end yet or not
            static virtual bool hasReachedEnd(ConcreteIterable) = 0;
    };

    template<template <class ValueType> class Policy, class ConcreteIterable, class ValueType>
    class Iterator {
        public:
            virtual ValueType getNext() = 0;
            virtual bool hasMore() = 0;
    };

    template<template<template <class Val> class Pol> class ConcreteIterator>
    class Iterable {
        public:
            template<template <class ValueType> class Policy>
            ConcreteIterator<Policy<ValueType>> createIterator() {
                return ConcreteIterator<Policy<ValueType>>();
            }
    };

    //========== Concrete Policies ==========//

    // Policy to loop from vector index 0 to end of vector
    template <class ValueType>
    class VectorForwardPolicy : IteratorPolicy<ValueType> {
        // Required for every policy
        static void step() {};
        static bool hasReachedEnd() {};
        // PANDA HAS ARRIVED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 🐼🐼 He cant save u
        // (DISASTER STRIKES!)
        
    };

    // Policy to loop from end of vector back to start
    template <class ValueType>
    class VectorBackwardsPolicy : IteratorPolicy<ValueType> {
        // Required for every policy
        void step() {};
        bool hasReachedEnd() {};
    };
    
    template<template <class ValueType> class Policy>
    class VectorIterator : Iterator<Policy<ValueType>, Vector, ValueType> {
        public:
            int getNext() {
                return 0;
            };

            bool hasMore() {
                return true;
            };
    };

    class Vector : public Iterable<VectorIterator> {
        int* data;
        int size;

        public:
            Vector(int* data, int size) {
                this->size = size;
                this->data = new int[size];
                for (int i = 0; i < size; i++) { this->data[i] = data[i]; }
            }
    };
};

int main() {
    using namespace lofty;

    int* data = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    Vector vec1(data, 10);

    auto iter = vec1.createIterator<VectorForwardPolicy>();

    while(iter.hasMore()) {
        std::cout << iter.getNext() << std::endl;
    }

    return 0;
}



namespace test {
    // template <long TO, long FROM>

    template< class Category, class T, class Distance = T, class Pointer = T* >
    class iterator {
        long porg;

        public:
            iterator(T _porg) : prog(_porg) {}

            iterator& operator++() { num = TO >= FROM ? num + 1: num - 1; return *this; }
            // iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }

            // bool operator==(iterator other) const { return num == other.num; }
            // bool operator!=(iterator other) const { return !(*this == other); }

            // long operator*() { return num; }

            // iterator traits
            using difference_type = long;
            using value_type = long;
            using pointer = const long*;
            // using iterator_category = std::forward_iterator_tag;
    };

    // iterator begin() { return FROM; }
    // iterator end() { return TO >= FROM? TO+1 : TO-1; }
};
