#include <iostream>


namespace hey {
    //========== Interfaces ==========//

    template<class TValue>
    struct DefaultStepPolicy {
        static TValue* step(TValue* current) {
            return current + 1;
        }
    };

    template<class TValue>
    struct StepForwardPolicy {
        static TValue* step(TValue* current) {
            return current + 1;
        }
    };

    template<class TValue>
    struct StepBackwardPolicy {
        static TValue* step(TValue* current) {
            return current - 1;
        }
    };

    // template<template <class> class A, class B>
    // class C {
    //     using D = A<B>;
        // using E = A<B>;
    // };

    // template<class TValue, class StepPolicy>
    // class Iterator {
    //     private:
    //         TValue* current;

    //     public:
    //         Iterator(TValue* current) : current(current) {}

    //         Iterator& operator++() {
    //             current = StepPolicy::step(current);
    //             return *this;
    //         }
    // };

    template<
        class TValue
    >
    class Iterable {
        public:
            template<
                template <class> class StepPolicy = DefaultStepPolicy
            >
            class iterator : public StepPolicy<TValue> {
                private:
                    TValue* current;

                public:
                    typedef StepPolicy<TValue> StepPolicyType;
                    iterator(TValue* current) : current(current)  {}

                    iterator& operator++() {
                        current = step(current);
                        return *this;
                    }

                    int operator*() {
                        return *current;
                    }
            };

        virtual iterator<> begin() = 0;
    };

    class Iterable2 {
        public:
            class iterator : public DefaultStepPolicy<int> {
                private:
                    int* current;

                public:
                    iterator(int* current) : current(current)  {}

                    iterator& operator++() {
                        current = DefaultStepPolicy<int>::step(current);
                        return *this;
                    }

                    int operator*() {
                        return *current;
                    }
            };

            virtual iterator begin() = 0;
    };
}

class Array : public hey::Iterable<int> {
    public:
        Array(int size) : size(size) {
            this->data = new int[size];
            this->data[0] = 69;
        }

        iterator<> begin() {
            return iterator<>(data);
        }

        // typename lofty::Iterable<T>::iterator begin() {
        //     return lofty::Iterable<T>::iterator(data);
        // }

    private:
        int* data;
        int size;
};

class Array2 : public hey::Iterable2 {
    public:
        Array2(int size) : size(size) {
            this->data = new int[size];
            this->data[0] = 69;
        }

        iterator begin() {
            return iterator(data);
        }

    private:
        int* data;
        int size;
};

int main() {
    using namespace hey;
    using namespace std;

    Array2 arr2(10);
    Array2::iterator itr2 = arr2.begin();

    Array arr(10);
    Array::iterator<> itr = arr.begin();

    cout << "Current Value: " << *itr << endl;

    return 0;
}