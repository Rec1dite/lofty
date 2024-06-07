#include <iostream>
#include "iter.h"

using namespace lofty;

//---------- Forward declarations ----------//
template<class> class TreeIterator;
class TreeBFSPolicy;
class TreeDFSPolicy;
class Tree;
class Node;

//========== Concrete Policies ==========//

template<class Policy>
class TreeIterator : public Iterator<TreeIterator, Tree, std::string, Policy> {
    private:
        Node* current;

    public:
        TreeIterator(Tree* tree)
            : Iterator<TreeIterator, Vector, int, Policy>(vec), current(NULL)
        {
            Policy::_goToStart(this, vec);
        }

        int getCurrent() {
            return this->structure->at(index);
        }

    friend Policy; // Automatically add policy as friend
};

struct Node {
    public:
        Node* left;
        Node* right;
        int data;
};

class Tree : public Structure<TreeIterator, Tree, int> {
    Node* root;

    public:
        Tree() {
            this->root = new Node();
        }

        int at(int index) {
            return this->data[index];
        }

    // TODO: Try to automate this
    friend class VectorForwardPolicy;
    friend class VectorBackwardsPolicy;
};

class VectorForwardPolicy {
    using This = VectorForwardPolicy;

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
class VectorBackwardsPolicy {
    using This = VectorBackwardsPolicy;

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



//========== Usage ==========//

int main() {
    int* data = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    Vector* vec1 = new Vector(data, 10);

    auto iterF = vec1->createIterator<VectorForwardPolicy>();
    auto iterB = vec1->createIterator<VectorBackwardsPolicy>();

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