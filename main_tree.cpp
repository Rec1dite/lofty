#include <iostream>
#include "iter.h"

using namespace lofty;

//---------- Forward declarations ----------//
template<class> class TreeIterator;
class Tree;
class Node;

//========== Concrete Policies ==========//

template<class Policy>
class TreeIterator : public Iterator<TreeIterator, Tree, std::string, Policy> {
    private:
        Node* current;

    public:
        TreeIterator(Tree* tree) : current(NULL), Iterator<TreeIterator, Tree, int, Policy>(vec) {}

        int getCurrent() {
            return this->structure->at(index);
        }

    friend Policy; // Automatically add policy as friend
};

struct Node {
    public:
        Node* left;
        Node* right;
        std::string data;
    
        Node() : left(NULL), right(NULL), data("") {};

        // Expr := "(Expr|Expr) | [data]"
        Node(std::string expr) : Node()
        {
            // If leaf node, set data
            if (expr[0] == '[') {
                this->data = expr.substr(1, expr.size() - 2);
                return;
            }
            else if (expr[0] == '(') {
                // Split into left and right
            }
        };

class Tree : public Structure<TreeIterator, Tree, std::string> {
    Node* root;

    public:
        Tree() { this->root = new Node(); }
        Tree(std::string expr) { this->root = new Node(expr); }

    // Breadth-first search policy
    class BFSPolicy {
        using ConcIterator = TreeIterator<BFSPolicy>;

        public:
            // Required for every policy
            static void _goToStart(ConcIterator* iterator, Tree* tree) {
                // iterator->index = 0;
            };

            // Required for every policy
            static void _step(ConcIterator* iterator, Tree* tree) {
                // if (iterator->index < vec->size) { iterator->index++; }
            };

            // Required for every policy
            static bool _hasReachedEnd(ConcIterator* iterator, Tree* tree) {
                // return iterator->index >= vec->size;
            };
    };

    // Depth-first search policy
    class DFSPolicy {
        using ConcIterator = TreeIterator<DFSPolicy>;

        public:
            // Required for every policy
            static void _goToStart(ConcIterator* iterator, Tree* tree) {
                // iterator->index = vec->size - 1;
            };

            // Required for every policy
            static void _step(ConcIterator* iterator, Tree* tree) {
                // if (iterator->index > 0) { iterator->index--; }
            };

            // Required for every policy
            static bool _hasReachedEnd(ConcIterator* iterator, Tree* tree) {
                // return iterator->index <= 0;
            };
    };

};



//========== Usage ==========//

int main() {
    int* data = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    Tree* tree = new Tree("(([a]|[b])|[c])");

    auto iterF = tree->createIterator<Tree::BFSPolicy>();
    auto iterB = tree->createIterator<Tree::DFSPolicy>();

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