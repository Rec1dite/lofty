#include <iostream>

using namespace std;

class Vector;
class VectorForwardPolicy;

template<class Policy>
class VectorIterator {
    private:
        int index;
        Vector* structure;

    public:
        VectorIterator(Vector* vec)
            // : Iterator<Vector, int, Policy>(vec)
            : structure(vec), index(0)
        {}

        int getNext() {
            Policy::_step(this, this->structure);
            return index;
        };

        bool hasMore() {
            return !Policy::_hasReachedEnd(this, this->structure);
        };

    friend class VectorForwardPolicy;
};

class Vector {
    int* data;
    int size;

    public:
        Vector(int* data, int size) {
            this->size = size;
            this->data = new int[size];
            for (int i = 0; i < size; i++) { this->data[i] = data[i]; }
        }

        template <class ConcretePolicy>
        VectorIterator<VectorForwardPolicy>* createIterator() {
            return new VectorIterator<VectorForwardPolicy>(this);
        }


    friend class VectorForwardPolicy;
};

// Policy to loop from vector index 0 to end of vector
class VectorForwardPolicy {
    using This = VectorForwardPolicy;

    public:
        // Required for every policy
        static void _step(VectorIterator<This>* iterator, Vector* vec) {
            if (iterator->index < vec->size) { iterator->index++; }
        };

        // Required for every policy
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
             