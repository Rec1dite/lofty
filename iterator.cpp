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
    // template<class ConcreteIterable, class ValueType>

    // Use Curiously Recursive Template Pattern to enforce definition of step() and hasReachedEnd() in derived classes
    // template<class CRTPDerivedPolicy, class ConcreteIterator>
    // class IteratorPolicy {
    //     public:
    //         // Describes how to move from one item in the iterable to the next
    //         static void step(ConcreteIterator iterator) {
    //             CRTPDerivedPolicy::_step(iterator);
    //         }
    //         // Determines whether we have reached the end yet or not
    //         static bool hasReachedEnd(ConcreteIterator iterator) {
    //             return CRTPDerivedPolicy::_hasReachedEnd(iterator);
    //         }

    //         // static virtual void step(ConcreteIterable) = 0; // TODO: Make this a Visitor
    //         // static virtual bool hasReachedEnd(ConcreteIterable) = 0;
    // };

    template<class ValueType>
    class Iterator {
        Visitor& visitor;

        public:
            void accept(Visitor& visitor) {
                this->visitor = visitor;
            }

            ValueType getNext() {
                visitor->getNext(this);
            };

            bool hasMore() {
                visitor->hasMore(this);
            };
    };

    template<class ConcreteIterator, class ValueType>
    class Iterable {
        public:

            template <class ConcreteVisitor>
            ConcreteIterator createIterator() {
                ConcreteIterator res();
                res->accept(ConcreteVisitor());

                // return ConcreteIterator<Policy>();
            }
    };

  
    template<class ValueType>
    class Visitor {
        public:
            virtual void getNext(Iterator<ValueType> iterator) = 0;
            virtual bool hasMore(Iterator<ValueType> iterator) = 0;
    };

    //========== Concrete Policies ==========//

    class VectorForwardVisitor : public Visitor<int> {
        void visit() {
            std::cout << "Visiting VectorForwardVisitor" << std::endl;
        }
    };

    // Policy to loop from vector index 0 to end of vector
    // class VectorForwardPolicy : IteratorPolicy<VectorForwardPolicy> {
    //     // Required for every policy
    //     static void _step() {
            
    //     };
    //     static bool _hasReachedEnd() { return false; };
    // };

    // Policy to loop from end of vector back to start
    // class VectorBackwardsPolicy : IteratorPolicy<VectorBackwardsPolicy> {
    //     // Required for every policy
    //     void _step(IntVecIterator iterator) {

    //     };
    //     bool _hasReachedEnd(IntVecIterator iterator) { return false; };
    // };

    class VectorIterator : Iterator<int> {
    };

    class Vector : public Iterable<VectorIterator, int> {
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

    auto iter = vec1.createIterator<VectorForwardVisitor>();

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
             