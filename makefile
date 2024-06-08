main:
	make build && make run && make clean

build:
	g++-14 -std=c++14 -o main main_range.cpp

run:
	./main

clean:
	rm -f *.o *.out main