// Last Updated: 2024-06-10

// Steps for usage:
// 1) Create some kind of data structure that you want to iterate through. This data structure must inherit from Structure<>
// 2) Create a generalized iterator class for that structure. This class must inherit from Iterator<>
// 3) Create one or more policies which define _how_ to iterate through the data structure. Each inherits from IteratorPolicy<>
// 4) Pass the policy into the Iterator<> to create a new Iterator<YourPolicy> class which can be used to iterate through the structure

//========== Interfaces ==========//

// Use policy to define how to iterate through a structure
// ValueType is the type of a single item within the Structure (that can be pointed to)


namespace lofty {
    struct NullType {};

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

            bool hasMore() {
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


    //========== Template Specialization for Self-Guided Iterators ==========//
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

            // Typedefs for iterator traits
            using iterator_category = std::input_iterator_tag;
            using value_type = long;
            using difference_type = std::ptrdiff_t;
            using pointer = const long*;
            using reference = const long&;

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

    // Uses Curiously Recursive Template Types (CRTP) for ConcreteStructure
    template<
        class ValueType,
        template <class> class ConcreteIterator,
        class ConcreteStructure = NullType
    >
    class Structure {
        public:
            using ValType = ValueType;

            template <class ConcretePolicy>
            ConcreteIterator<ConcretePolicy> createIterator() {
                return ConcreteIterator<ConcretePolicy>(static_cast<ConcreteStructure*>(this));
            }
    };

    //========== Template Specialization for Self-Guided Iterators ==========//
    template<
        class ValueType,
        template <class TValue> class ConcreteIterator
    >
    class Structure<ValueType, ConcreteIterator, NullType> {
        public:
            template <class ConcretePolicy>
            ConcreteIterator<ConcretePolicy> createIterator() {
                return ConcreteIterator<ConcretePolicy>();
            }
    };

} // namespace lofty

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
