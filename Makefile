all: simulate

simulate: header.o mpcd.o publish.o main.o 
	g++ main.cpp mpcd.cpp publish.cpp -o main.exe

header.o: mpcd.h
	g++ -c mpcd.h

mpcd.o: mpcd.cpp
	g++ -c mpcd.cpp

calc.o: calc.cpp
	g++ -c calc.cpp

publish.o: publish.cpp
	g++ -c publish.cpp

main.o: main.cpp
	g++ -c main.cpp	
