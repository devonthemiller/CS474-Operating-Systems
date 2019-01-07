matrix_multiply: matrix.o
	g++ -pthread -std=c++11 matrix.o -o matrix_multiply
	rm matrix.o

matrix.o:
	g++ -pthread -std=c++11 -c matrix.cpp
