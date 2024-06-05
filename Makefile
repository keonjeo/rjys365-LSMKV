
LINK.o = $(LINK.cc)
CXXFLAGS = -std=c++14 -Wall -g

all: correctness persistence benchmark_test

correctness: kvstore.o correctness.o
	g++ -o correctness kvstore.o correctness.o -I./header

persistence: kvstore.o persistence.o
	g++ -o persistence kvstore.o persistence.o -I./header

benchmark_test: kvstore.o benchmark_test.o
	g++ -o benchmark_test kvstore.o benchmark_test.o -I./header

clean:
	-rm -f correctness persistence benchmark_test *.o
