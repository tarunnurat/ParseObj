CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Werror -g -O0

EXE = testing

.PHONY: clean

$(EXE): parse.cpp $(wildcard *.h)
	$(CXX) $(CXXFLAGS) -o $(EXE) parse.cpp

clean:
	-rm -rf $(EXE) *.o
	-rm -f *.obj
