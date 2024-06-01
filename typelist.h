namespace lofty {
    #define TYPELIST_1(T1) Typelist<T1, NullType>
    #define TYPELIST_2(T1, T2) Typelist<T1, TYPELIST_1(T2)>
    #define TYPELIST_3(T1, T2, T3) Typelist<T1, TYPELIST_2(T2, T3)>
    #define TYPELIST_4(T1, T2, T3, T4) Typelist<T1, TYPELIST_3(T2, T3, T4)>
    #define TYPELIST_5(T1, T2, T3, T4, T5) Typelist<T1, TYPELIST_4(T2, T3, T4, T5)>

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
        enum { value = 0 };
    };

    template <class Head, class Tail>
    struct Length<Typelist<Head, Tail>> {
        enum { value = 1 + Length<Tail>::value };
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
        enum { value = -1 };
    };

    template<class Head, class Tail>
    struct IndexOf<Typelist<Head, Tail>, Head> {
        enum { value = 0 };
    };

    template<class Head, class Tail, class T>
    struct IndexOf<Typelist<Head, Tail>, T> {
        private:
            enum { temp = IndexOf<Tail, T>::value };
        public:
            enum { value = temp == -1 ? -1 : temp + 1 };
    };


    // ============================================================
    // UTILS
    // ============================================================

    // ========== Equals ==========

    template <class T, class U>
    struct Equals {
        enum { value = 0 };
    };

    template <class T>
    struct Equals<T, T> {
        enum { value = 1 };
    };

} // namespace lofty
