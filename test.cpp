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

int main() {
    C<A<B>> c;
    return 0;
}



template<class Z>
class X {
    private:
        int a;
    protected:
        int b;
    public:
        int c;
    
    friend class Y;
};

class X<float> {
};

class Y {
    void test(X<int>* x) {
    }
};