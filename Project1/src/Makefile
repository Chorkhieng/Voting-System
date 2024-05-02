CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: main

main: main.o CPLSystem.o Candidate.o ElectionSystem.o OPLSystem.o Party.o Results.o TieBreaker.o
	$(CXX) $(CXXFLAGS) main.o CPLSystem.o Candidate.o ElectionSystem.o OPLSystem.o Party.o Results.o TieBreaker.o -o main

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

CPLSystem.o: CPLSystem.cpp CPLSystem.h
	$(CXX) $(CXXFLAGS) -c CPLSystem.cpp -o CPLSystem.o

Candidate.o: Candidate.cpp Candidate.h
	$(CXX) $(CXXFLAGS) -c Candidate.cpp -o Candidate.o

ElectionSystem.o: ElectionSystem.cpp ElectionSystem.h
	$(CXX) $(CXXFLAGS) -c ElectionSystem.cpp -o ElectionSystem.o

OPLSystem.o: OPLSystem.cpp OPLSystem.h
	$(CXX) $(CXXFLAGS) -c OPLSystem.cpp -o OPLSystem.o

Party.o: Party.cpp Party.h
	$(CXX) $(CXXFLAGS) -c Party.cpp -o Party.o

Results.o: Results.cpp Results.h
	$(CXX) $(CXXFLAGS) -c Results.cpp -o Results.o

TieBreaker.o: TieBreaker.cpp TieBreaker.h
	$(CXX) $(CXXFLAGS) -c TieBreaker.cpp -o TieBreaker.o

# for testing parts
# run test on Candidate class
UnitTests:
	g++ -o UnitTests UnitTests.cpp Candidate.cpp Party.cpp TieBreaker.cpp CPLSystem.cpp OPLSystem.cpp ElectionSystem.cpp Results.cpp -lgtest -lgtest_main -pthread

unittest: UnitTests
	./UnitTests

# run test on Candidate class
SystemTests:
	g++ -o SystemTests SystemTests.cpp Candidate.cpp Party.cpp TieBreaker.cpp CPLSystem.cpp OPLSystem.cpp ElectionSystem.cpp Results.cpp -lgtest -lgtest_main -pthread

systemtest: SystemTests
	./SystemTests

# clean update .o files and object
clean:
	rm -f *.o main UnitTests SystemTests

run: main
	./main $(FILE)