all: run clean

run: vec range tree

vec:
	g++ -std=c++14 -o main_vec main_vec.cpp
	./main_vec

range:
	g++ -std=c++14 -o main_range main_range.cpp
	./main_range

tree:
	g++ -std=c++14 -o main_tree main_tree.cpp
	./main_tree

clean:
	rm -f *.o *.out main_vec main_range main_tree