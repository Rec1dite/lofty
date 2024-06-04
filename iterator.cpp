namespace lofty {

    class Iterator {
        public:
            virtual Iterable& getNext() = 0;
            virtual bool hasMore() = 0;
    };

    class Iterable {
        public:
            virtual Iterator& createIterator() = 0;


    };
};

class MyConcreteIterator : public lofty::Iterator {
    public:
        lofty::Iterable& getNext() {
            return MyConcreteIterable();
        }
        bool hasMore() {
            return true;
        }
};

class MyConcreteIterable : public lofty::Iterable {
    public:
        lofty::Iterator& createIterator() {
            return MyConcreteIterator();
        }
};


int main() {
    MyConcreteIterable iterable;
    lofty::Iterator& iterator = iterable.createIterator();

    while (iterator.hasMore()) {
        lofty::Iterable& next = iterator.getNext();
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