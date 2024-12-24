CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread

TARGET = main
OBJ =  Baker.o CustomerQueue.o main.o 

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)
	
CustomerQueue.o: CustomerQueue.cpp CustomerQueue.hpp
	$(CXX) $(CXXFLAGS) -c CustomerQueue.cpp

Baker.o: Baker.cpp Baker.hpp CustomerQueue.hpp
	$(CXX) $(CXXFLAGS) -c Baker.cpp


main.o: main.cpp CustomerQueue.hpp Baker.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp


clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
