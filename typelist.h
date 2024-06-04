namespace lofty {
    #define TYPELIST_1(T1) Typelist<T1, NullType>
    #define TYPELIST_2(T1, T2) Typelist<T1, TYPELIST_1(T2)>
    #define TYPELIST_3(T1, T2, T3) Typelist<T1, TYPELIST_2(T2, T3)>
    #define TYPELIST_4(T1, T2, T3, T4) Typelist<T1, TYPELIST_3(T2, T3, T4)>
    #define TYPELIST_5(T1, T2, T3, T4, T5) Typelist<T1, TYPELIST_4(T2, T3, T4, T5)>
    #define sci static constexpr int

    class NullType {};
    struct EmptyType {};

    template <class H, class T>
    struct Typelist {
        using Head = H;
        using Tail = T;
    };

    // ========== Length ==========

    template <class TList>
    struct Length;

    template <>
    struct Length<NullType> {
        sci value = 0;
    };

    template <class Head, class Tail>
    struct Length<Typelist<Head, Tail>> {
        sci value = 1 + Length<Tail>::value;
    };

    // ========== TypeAt ==========

    template <class TList, unsigned int index>
    struct TypeAt;

    template <int index>
    struct TypeAt<NullType, index> {
        using Result = NullType;
    };

    template <class Head, class Tail>
    struct TypeAt<Typelist<Head, Tail>, 0> {
        using Result = Head;
    };

    template <class Head, class Tail, unsigned int index>
    struct TypeAt<Typelist<Head, Tail>, index> {
        using Result = typename TypeAt<Tail, index - 1>::Result;
    };


    // ========== IndexOf ==========

    template <class TList, class T>
    struct IndexOf;

    template<class T>
    struct IndexOf<NullType, T> {
        sci value = -1;
    };

    template<class Head, class Tail>
    struct IndexOf<Typelist<Head, Tail>, Head> {
        sci value = 0;
    };

    template<class Head, class Tail, class T>
    struct IndexOf<Typelist<Head, Tail>, T> {
        private:
            sci temp = IndexOf<Tail, T>::value;
        public:
            sci value = temp == -1 ? -1 : temp + 1;
    };


    // ============================================================
    // UTILS
    // ============================================================

    // ========== Equals ==========

    struct True {};
    struct False {};

    union Boolean {
    };

    template <class T>
    struct Equals<T, T> {
        using Result = True;
    };

    template <class T, class U>
    struct Equals {
        using Result = False;
    };

    // ========== GenScatterHierarchy ==========

    // Apply each type in a Typelist to a basic template Unit<T>
    template<class TList, template<class> class Unit>
    class GenScatterHierarchy;

    template<class X, class Xs, template<class> class Unit>
    class GenScatterHierarchy<Typelist<X, Xs>, Unit>
        : public GenScatterHierarchy<X, Unit>
        , public GenScatterHierarchy<Xs, Unit>
    {
        public:
            using TList = Typelist<X, Xs>;
            using LeftBase = GenScatterHierarchy<X, Unit>;
            using RightBase = GenScatterHierarchy<Xs, Unit>;
    }; 

    // atomic type (not a typelist) -> Unit<AtomicType>
    template<class AtomicType, template<class> class Unit>
    class GenScatterHierarchy : public Unit<AtomicType> {
        public:
            using LeftBase = Unit<AtomicType>;
    };

    // NullType base case
    template<template<class> class Unit>
    class GenScatterHierarchy<NullType, Unit> {};

}; // namespace lofty


using namespace lofty;

using List = TYPELIST_4(int, char, double, float);

sci len = Length<NullType>::value;