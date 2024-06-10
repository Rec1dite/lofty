<p align="center">
  <img src="https://static.wikia.nocookie.net/btb/images/a/a1/LoftyStopMotion.png" width="350" title="lofty">
</p>

<h1 align="center">Lofty</h1>

## What is lofty?

Lofty is a C++ library with a generic implementation of the GoF's `Iterator`
pattern. It provides a way to iterate over a collection of objects without
exposing the underlying representation of the collection.

The focus was on the exploration of C++ compile time template metaprogramming
techniques to implement the pattern.

Some of the features:
- Unopinionated iterator design. Any container can be used with lofty
- Policy base design to allow customization of the iterator behavior
- Optionally compatible with std::iterator
- Optionally compatible with range-based for loops

## How to use lofty?

To use lofty, you need to include the `lofty.h` header file in your project.

## Dependencies

To run the examples, you need to have a C++ compiler that supports at least
C++14.

## How to run the library examples

In the terminal, run the following commands:

To run all the code examples
```bash
make
```

To run the vector example
```bash
make vec
```

To run the binary tree example
```bash
make tree
```

To run the range example
```bash
make range
```
