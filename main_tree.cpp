#include <iostream>
#include "lofty.h"
#include "utils.h"
#include <queue>
#include <stack>
#include <random>
#include <sstream>

using namespace lofty;

std::random_device rd;
    
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 100);

//---------- Forward declarations ----------//
template<class> class TreeIterator;
class Tree;

//========== Concrete Policies ==========//

struct Node {
    public:
        Node* left;
        Node* right;
        float data;

        Node() : left(NULL), right(NULL), data(0) {};

        Node(float value) : left(NULL), right(NULL), data(value) {};
};

template<class Policy>
class TreeIterator : public Iterator<float, Policy, TreeIterator, Tree> {
    private:
        Node* current;

    public:
        TreeIterator(Tree* tree) : current(NULL), Iterator<float, Policy, TreeIterator, Tree>(tree) {
            Policy::_goToStart(this, tree);
        }

        float getCurrent() {
            return current->data;
        }
    friend Policy; // Automatically add policy as friend
};

class Tree : public Structure<float, TreeIterator, Tree> {
    Node* root;

    public:
        Tree(float depth) { 
            this->root = full(depth);
            std::cout << "Tree created" << std::endl;
        }

    void printBinaryTree(Node* node, float depth = 0) {
        if (node == nullptr) return;

        std::string alignDots = "";
        for (int i = 0; i < 8-depth; i++) {
            alignDots += "...";
        }

        // Print current node
        std::cout << "\033[90m";
        for (int i = 0; i < depth; i++) {
            std::cout << " | ";
        }
        std::cout << "[" << depth + 1 << "]" << alignDots << "\033[94m " << node->data << "\033[0m" << std::endl;

        // Print left subtree
        printBinaryTree(node->left, depth + 1);

        // Print right subtree
        printBinaryTree(node->right, depth + 1);
    }

    void print(){
        printBinaryTree(root);
    }

    // Breadth-first search policy
    class BFSPolicy {
        using ConcIterator = TreeIterator<BFSPolicy>;
        protected: 
            std::queue<Node*> queue;

        public:
            // Required for every policy
            static void _goToStart(ConcIterator* iterator, Tree* tree) {
                iterator->current = tree->root;
                iterator->queue = std::queue<Node*>();
                iterator->queue.push(iterator->current);
            };

            // Required for every policy
            static void _step(ConcIterator* iterator, Tree* tree) {
                iterator->queue.pop();
                if (iterator->current->left != NULL) { 
                    iterator->queue.push(iterator->current->left);
                    iterator->queue.push(iterator->current->right);
                 }
                 
                if (!iterator->queue.empty())
                iterator->current = iterator->queue.front();
            };

            // Required for every policy
            static bool _hasReachedEnd(ConcIterator* iterator, Tree* tree) {
                return iterator->queue.empty();
            };
    };

    // Depth-first search policy
    class DFSPolicy {
        using ConcIterator = TreeIterator<DFSPolicy>;

        protected: 
            std::stack<Node*> stack;

        public:
            // Required for every policy
            static void _goToStart(ConcIterator* iterator, Tree* tree) {
                iterator->current = tree->root;
                iterator->stack = std::stack<Node*>();
                iterator->stack.push(iterator->current);
            };

            // Required for every policy
            static void _step(ConcIterator* iterator, Tree* tree) {
                iterator->stack.pop();
                if (iterator->current->left != NULL) { 
                    iterator->stack.push(iterator->current->right);
                    iterator->stack.push(iterator->current->left);
                 }

                if (!iterator->stack.empty())
                iterator->current = iterator->stack.top();
            };

            // Required for every policy
            static bool _hasReachedEnd(ConcIterator* iterator, Tree* tree) {
                return iterator->stack.empty();
            };
    };



    private: 
        Node* full(int depth){
            if (depth == 0) {
                return new Node(dis(gen));
            }

            Node* node = new Node(dis(gen));
            node->left = full(depth - 1);
            node->right = full(depth - 1);
            return node;
        }

};



//========== Usage ==========//

int main() {
    Tree* tree = new Tree(4);

    auto iterF = tree->createIterator<Tree::BFSPolicy>();
    auto iterB = tree->createIterator<Tree::DFSPolicy>();

    heading("Tree", 1, 1);
    tree->print();

    subheading("Breadth-first search", 1, 1);
    while(iterF()) {
        std::cout << *iterF++ << " ";
    }

    subheading("Depth-first search", 2, 1);
    while(iterB()) {
        std::cout << *iterB++ << " ";
    }

    std::cout << std::endl << std::endl;

}