CPPFLAGS += -Wall -Wextra -std=c++17
SRC_ALL = $(wildcard *.cpp)
SRC = $(filter-out cli.cpp, $(SRC_ALL))
OBJ = $(SRC:%.cpp=%.o)
paint: $(OBJ)
	$(CXX) $^ -o $@

cli: cli.o
	$(CXX) $^ -o  $@.out

clean:
	$(RM) -rf $(OBJ)
