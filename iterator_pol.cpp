#include <iostream>

// Steps for usage:
// 1) Create some kind of data structure that you want to iterate through. This data structure must inherit from Structure<>
// 2) Create a generalized iterator class for that structure. This class must inherit from Iterator<>
// 3) Create one or more policies which define _how_ to iterate through the data structure. Each inherits from IteratorPolicy<>
// 4) Pass the policy into the Iterator<> to create a new Iterator<YourPolicy> class which can be used to iterate through the structure

using namespace std;


//========== Interfaces ==========//

// Use policy to define how to iterate through a structure (to be converted to Visitor)
// ValueType is the type of a single item within the Structure (that can be pointed to)
// template<class ConcreteStructure, class ValueType>

template<class BaseClass>
class InheritedClass : public BaseClass {};

//---------- Forward declarations ----------//
template<template<class> class, class, class, class> class Iterator;
template<template<class> class, class, class> class Structure;
template<class> class VectorIterator;
class VectorForwardPolicy;
class Vector;

template<template<class Pol> class ConcreteIterator, class ConcreteStructure, class ValueType, class Policy>
class Iterator {

    private:
        ConcreteStructure* structure;

    public:
        using ValType = ValueType;
        using PolType = Policy;
        using ConcIter = ConcreteIterator<Policy>;

        Iterator(ConcreteStructure* iterble) : structure(iterble) {}

        ValueType getNext() {
            Policy::_step(static_cast<ConcIter*>(this), this->structure);
            return 0;
        };

        bool hasMore() {
            // return true;
            return Policy::_hasReachedEnd(static_cast<ConcIter*>(this), this->structure);
        };
};

// Uses CRTP for ConcreteStructure
template<template<class> class ConcreteIterator, class ConcreteStructure, class ValueType>
class Structure {
    public:
        // using IteratorType = InheritedClass<Iterator<ValueType>>;
        // template<class ConcPol>
        // using IteratorType = ConcreteIterator<ConcreteStructure, ValueType, ConcPol>;


        // TODO: Look into generating the class directly inside the iterable
        // Then could probide a template parameter to Structure<> which defaults to NullType
        // If the user provides a class to this parameter, then it uses that as the ConcreteIterator instead
        // template<class ConcretePolicy>
        // class CustomIterator : public Iterator<ConcreteStructure, ValueType, ConcretePolicy> {
        // };

        template <class ConcretePolicy>
        ConcreteIterator<ConcretePolicy>* createIterator() {
            return new ConcreteIterator<ConcretePolicy>(static_cast<ConcreteStructure*>(this));
        }
};

//========== Concrete Policies ==========//
// Use GenScatterHierarchy to build a unified policy from a set of other policies
// E.g. First policy defines getNext(), 2nd defines hasMore(), etc.

// TODO: VectorIterator should take a TypeList of config types E.g. ValType, StepType, EndType
template<class Policy>
class VectorIterator : public Iterator<VectorIterator, Vector, int, Policy> {
    private:
        int index;
        // Vector* structure;

    public:
        VectorIterator(Vector* vec)
            // : Iterator<Vector, int, Policy>(vec), index(0) //
            : Iterator<VectorIterator, Vector, int, Policy>(vec), index(0) //
        {}

    friend class VectorForwardPolicy;
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

    friend class VectorForwardPolicy;
};

// Policy to loop from vector index 0 to end of vector
class VectorForwardPolicy {
    using This = VectorForwardPolicy;

    public:
        // Required for every policy
        // template<class IteratorType>
        static void _step(VectorIterator<This>* iterator, Vector* vec) {
            if (iterator->index < vec->size) { iterator->index++; }
        };

        // Required for every policy
        // template<class IteratorType>
        static bool _hasReachedEnd(VectorIterator<This>* iterator, Vector* vec) {
            return iterator->index >= vec->size;
        };
};



//========== Usage ==========//

int main() {
    int* data = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    Vector* vec1 = new Vector(data, 10);

    auto iter = vec1->createIterator<VectorForwardPolicy>(); //
    // auto iter = new VectorIterator<VectorForwardPolicy>(vec1);

    // auto iter1 = vec1.createIterator<APolicy>();
    // auto iter2 = vec1.createIterator<BPolicy>();

    while(iter->hasMore()) {
        std::cout << iter->getNext() << std::endl;
    }

    return 0;
}

// Porg:                                
//                                      )\   /|        //
//                                   .-/'-|_/ |        //
//                __            __,-' (   / \/         //
//            .-'"  "'-..__,-'""          -o.`-._      //
//           /                                   '/    //
//   *--._ ./                                 _.--     //
//         |                              _.-'         //
//         :                           .-/             //
//          \                       )_ /               //
//           \                _)   / \(                //
//             `.   /-.___.---'(  /   \\               //
//              (  /   \\       \(     L\              //
//               \(     L\       \\                    //
//                \\              \\                   //
//                 L\              L\                  //
// "All porg are equal, but some porg are more equal then others"
//  - Peorge Porgwell (Porg Fram) - 1945 


// Its just me and Porg over here, tuff times are lasting.
// Dino hasnt spoken to a human in over 50 days. ;(
// I fear for his sanity
// What will become of us, now that we are stuck here with him.
// If only he were to commit this code, then Porg will bless the codebase. 
             