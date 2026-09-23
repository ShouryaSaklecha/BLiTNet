CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Werror -Isrc -Itests
BUILD    := build
HEADERS  := $(wildcard src/*.hpp tests/*.hpp)

TESTS := test_empty test_check

# build every test, run each one, stop at the first failure
test: $(TESTS:%=$(BUILD)/%)
	@for t in $^; do ./$$t || exit 1; done

$(BUILD)/%: tests/%.cpp $(HEADERS)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -rf $(BUILD)

.PHONY: test clean
