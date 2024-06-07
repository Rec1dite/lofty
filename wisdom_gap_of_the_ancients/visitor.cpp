//Visit your mom with this code. 

#include <iostream>


#define DEFINE_VISITABLE() \
virtual ReturnType Accept(BaseVisitor& guest) \
{ return AcceptImpl(*this, guest); }


class BaseVisitor
{
    public:
        virtual ~BaseVisitor() {}
};


template <class T, typename R = void>
class Visitor
{
    public:
    typedef R ReturnType;
    virtual ReturnType Visit(T&) = 0;
};


// class SomeVisitor :
// public BaseVisitor, // required
// public Visitor<DocElement>, // a visitor for each type you want to visit
// public Visitor<Paragraph>
// {
//     public:
//         void Visit(DocElement&); // visit a int
//         void Visit(Paragraph &); // visit a string
// };


template <typename R = void>
class BaseVisitable
{
    public:
        typedef R ReturnType;
        virtual ~BaseVisitable() {}
        virtual ReturnType Accept(BaseVisitor&) = 0;
    protected: // Give access only to the hierarchy
        template <class T>
        static ReturnType AcceptImpl(T& visited, BaseVisitor& guest)
        {
            // Apply the Acyclic Visitor
            if (Visitor<T>* p = dynamic_cast<Visitor<T>*>(&guest))
                {
                    return p->Visit(visited);
                }
            return ReturnType();
        }
};



// Test drive : 

class DocElement : public BaseVisitable<>
{
public:
    DEFINE_VISITABLE()
};


class Paragraph : public DocElement
{
public:
    DEFINE_VISITABLE()
};


class MyConcreteVisitor :
public BaseVisitor, // required
public Visitor<DocElement>,// visits DocElements
public Visitor<Paragraph> // visits Paragraphs
{
    public:
        void Visit(DocElement&) { std::cout << "Visit(DocElement&) \n"; }
        void Visit(Paragraph&) { std::cout << "Visit(Paragraph&) \n"; }
};

int main()
{
    MyConcreteVisitor visitor;
    Paragraph par;
    DocElement* d = &par; // "hide" the static type of 'par'
    d->Accept(visitor);
}