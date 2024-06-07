#include <iostream>
#include "typelist.h"

// template<template<class B> class A> 
// class C : public B {
//     typedef B D;
//     typedef A E;
// };

class B {};

template<class _B>
class A {
    public:
        typedef _B __B;
};

template<class _A> 
class C : public B {
    typedef typename _A::__B D;
    typedef _A E;
};



template<typename... Types>
struct TypeList {};


template<typename T, typename Ts>
struct FriendMaker;

template<typename T, typename Head, typename... Tail>
struct FriendMaker<T, TypeList<Head, Tail...>> {
    friend Head;
    FriendMaker() {
        FriendMaker<T, TypeList<Tail...>> next;
    }
};

template<class W>
class X {
    private:    int a = 1;
    protected:  int b = 2;
    public:     int c = 3;
    
    friend W;
};

class Y {
    private:    int a = 1;
    protected:  int b = 2;
    public:     int c = 3;

    FriendMaker<Y, TypeList<X<Y>>> friendMaker;
};

class U;

using Co = lofty::Typelist<U, lofty::NullType>;
using FriendlyY = lofty::AddFriends<Y, Co>;

class U {
    public:
        void accessX(X<U>* x) {
            std::cout << "A: " << x->a << std::endl;
            std::cout << "B: " << x->b << std::endl;
            std::cout << "C: " << x->c << std::endl;
        }

        void accessFriendlyY(FriendlyY* y) {
            // TODO: Fix; can't access a because private prevents inheritance in AddFriends<>
            // std::cout << "A: " << y->a << std::endl;
            std::cout << "B: " << y->b << std::endl;
            std::cout << "C: " << y->c << std::endl;
        }

        void accessY(Y* y) {
            // std::cout << "A: " << y->a << std::endl;
            // std::cout << "B: " << y->b << std::endl;
            // std::cout << "C: " << y->c << std::endl;
        }
};

int main() {
    X<U> x;
    Y y;
    FriendlyY fy;

    U u;
    u.accessX(&x);

    std::cout << std::endl;

    u.accessFriendlyY(&fy);

    return 0;
}