# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = logai-store

SRCS = main.cpp engine/memtable.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	del $(TARGET).exe
