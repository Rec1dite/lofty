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

struct NullType {};
namespace lofty {

    //---------- Forward declarations ----------//
    template<class, class, template<class> class, class> class Iterator;
    template<class, template<class> class, class> class Structure;

    template<
        class ValueType,
        class Policy,
        template <class Pol> class ConcreteIterator,
        class ConcreteStructure = NullType
    >
    class Iterator : public Policy {

        protected:
            ConcreteStructure* structure;

        public:
            using ValType = ValueType;
            using PolType = Policy;
            using ConcIter = ConcreteIterator<Policy>;
            using Base = Iterator<ValueType, Policy, ConcreteIterator, ConcreteStructure>;

            #define THIS_ITER static_cast<ConcreteIterator<Policy>*>(this)

            Iterator(ConcreteStructure* iterable) : structure(iterable) {
                Policy::_goToStart(THIS_ITER, this->structure);
            }

            //----- Interface -----//
            virtual ValueType getCurrent() = 0;

            ValueType pop() {
                ValueType res = getCurrent();
                Policy::_step(THIS_ITER, this->structure);
                return res;
            };

            ConcIter& next() {
                Policy::_step(THIS_ITER, this->structure);
                return *THIS_ITER;
            };

            bool hasMore() { // TODO: Try make this const/readonly
                return !Policy::_hasReachedEnd(THIS_ITER, this->structure);
            };

            //----- Operator overloads -----//
            bool operator()() { return this->hasMore(); }

            ConcIter& operator++() { // Prefix increment
                Policy::_step(THIS_ITER, this->structure);
                return *THIS_ITER;
            };

            ConcIter operator++(int) { // Postfix increment
                auto res = *THIS_ITER;
                Policy::_step(THIS_ITER, this->structure);
                return res;
            };

            ValueType operator*() { return getCurrent(); }

            #undef THIS_ITER
    };


    template<
        class ValueType,
        class Policy,
        template <class Pol> class ConcreteIterator
    >
    class Iterator<ValueType, Policy, ConcreteIterator, NullType> : public Policy {
        public:
            using ValType = ValueType;
            using PolType = Policy;
            using ConcIter = ConcreteIterator<Policy>;
            using Base = Iterator<ValueType, Policy, ConcreteIterator, NullType>;

            #define THIS_ITER static_cast<ConcreteIterator<Policy>*>(this)

            Iterator() {
                Policy::_goToStart(THIS_ITER);
            }

            ValueType getNext() {
                Policy::_step(THIS_ITER);
                return getCurrent();
            };

            bool hasMore() {
                return !Policy::_hasReachedEnd(THIS_ITER);
            };

            virtual ValueType getCurrent() = 0;

            // Operator overloads
            bool operator()() { return this->hasMore(); }

            ValueType operator++() {
                return getNext();
            };
            ValueType operator++(int) {
                ValueType res = getCurrent();
                getNext();
                return res;
            };
            ValueType operator*() { return getCurrent(); }

            #undef THIS_ITER 
    };

    // Uses CRTP for ConcreteStructure
    template<
        class ValueType,
        template <class> class ConcreteIterator,
        class ConcreteStructure = NullType
    >
    class Structure {
        public:
            // TODO: Look into generating the class directly inside the iterable
            // Then could provide a template parameter to Structure<> which defaults to NullType
            // If the user provides a class to this parameter, then it uses that as the ConcreteIterator instead
            // template<class ConcretePolicy>
            // class CustomIterator : public Iterator<ConcreteStructure, ValueType, ConcretePolicy> {
            // };

            using ValType = ValueType;

            template <class ConcretePolicy>
            ConcreteIterator<ConcretePolicy> createIterator() {
                return ConcreteIterator<ConcretePolicy>(static_cast<ConcreteStructure*>(this));
            }
    };

    template<
        class ValueType,
        template <class TValue> class ConcreteIterator
    >
    class Structure<ValueType, ConcreteIterator, NullType> {
        public:
            // TODO: Look into generating the class directly inside the iterable
            // Then could provide a template parameter to Structure<> which defaults to NullType
            // If the user provides a class to this parameter, then it uses that as the ConcreteIterator instead
            // template<class ConcretePolicy>
            // class CustomIterator : public Iterator<ConcreteStructure, ValueType, ConcretePolicy> {
            // };

            template <class ConcretePolicy>
            ConcreteIterator<ConcretePolicy> createIterator() {
                return ConcreteIterator<ConcretePolicy>();
            }
    };


    // Stateless structure that can be used as a placeholder for self-guided iterators
    // template<template<class Pol> class ConcreteIterator, class ValueType>
    // class NullStructure : public Structure<ConcreteIterator, NullStructure<ConcreteIterator, ValueType>, ValueType> {};
};

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
