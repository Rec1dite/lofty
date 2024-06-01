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