// Steps for usage:
// 1) Create some kind of data structure that you want to iterate through. This data structure must inherit from Structure<>
// 2) Create a generalized iterator class for that structure. This class must inherit from Iterator<>
// 3) Create one or more policies which define _how_ to iterate through the data structure. Each inherits from IteratorPolicy<>
// 4) Pass the policy into the Iterator<> to create a new Iterator<YourPolicy> class which can be used to iterate through the structure

//========== Interfaces ==========//

// Use policy to define how to iterate through a structure (to be converted to Visitor)
// ValueType is the type of a single item within the Structure (that can be pointed to)
// template<class ConcreteStructure, class ValueType>

template<typename... Types>
struct TypeList {};


template<typename T, typename TypeList>
struct FriendMaker;

template<typename T, typename Head, typename... Tail>
struct FriendMaker<T, TypeList<Head, Tail...>> {
    friend Head;
    FriendMaker() {
        FriendMaker<T, TypeList<Tail...>> next;
    }
};

template<typename T>
struct FriendMaker<T, TypeList<>> {};

namespace lofty {

    //---------- Forward declarations ----------//
    template<template<class> class, class, class, class> class Iterator;
    template<template<class> class, class, class> class Structure;

    template<template<class Pol> class ConcreteIterator, class ConcreteStructure, class ValueType, class Policy>
    class Iterator : public Policy {

        protected:
            ConcreteStructure* structure;

        public:
            using ValType = ValueType;
            using PolType = Policy;
            using ConcIter = ConcreteIterator<Policy>;
            using Base = Iterator<ConcreteIterator, ConcreteStructure, ValueType, Policy>;

            #define THIS_ITER static_cast<ConcreteIterator<Policy>*>(this)

            Iterator(ConcreteStructure* iterable) : structure(iterable) {
                Policy::_goToStart(THIS_ITER, this->structure);
            }

            ValueType getNext() {
                Policy::_step(THIS_ITER, this->structure);
                return getCurrent();
            };

            bool hasMore() { // TODO: Try make this const/readonly
                return !Policy::_hasReachedEnd(THIS_ITER, this->structure);
            };

            virtual ValueType getCurrent() = 0;

            // Operator overloads
            bool operator()() { return this->hasMore(); }

            ValueType operator++(int) {
                return getNext();
            };
            ValueType operator++() {
                ValueType res = getCurrent();
                getNext();
                return res;
            };
            ValueType operator*() { return getCurrent(); }

            #undef THIS_ITER
    };

    // Uses CRTP for ConcreteStructure
    template<template<class> class ConcreteIterator, class ConcreteStructure, class ValueType>
    class Structure {
        public:
            // TODO: Look into generating the class directly inside the iterable
            // Then could provide a template parameter to Structure<> which defaults to NullType
            // If the user provides a class to this parameter, then it uses that as the ConcreteIterator instead
            // template<class ConcretePolicy>
            // class CustomIterator : public Iterator<ConcreteStructure, ValueType, ConcretePolicy> {
            // };

            template <class ConcretePolicy>
            ConcreteIterator<ConcretePolicy>* createIterator() {
                return new ConcreteIterator<ConcretePolicy>(static_cast<ConcreteStructure*>(this));
            }
    };

};