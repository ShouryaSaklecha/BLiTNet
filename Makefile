CXX      := g++
CXXFLAGS := -O2 -std=c++17 -march=native -Isrc -Wall -Wextra
SRC      := $(wildcard src/*.cpp)
OBJ      := $(SRC:.cpp=.o)
BIN      := build/blitnet

$(BIN): $(OBJ)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -f src/*.o $(BIN)

.PHONY: run clean
