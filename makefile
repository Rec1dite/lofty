all: build run clean

build: vec range tree

vec:
	g++ -std=c++14 -o main_vec main_vec.cpp

range:
	g++ -std=c++14 -o main_range main_range.cpp

tree:
	g++ -std=c++14 -o main_tree main_tree.cpp

run:
	./main_vec
	./main_range
	./main_tree

clean:
	rm -f *.o *.out main_vec main_range main_tree