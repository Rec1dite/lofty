main:
	make build && make run && make clean

run:
	./main

build:
	g++-14 -std=c++11 gay.cpp -o main

clean:
	rm -f *.o main
