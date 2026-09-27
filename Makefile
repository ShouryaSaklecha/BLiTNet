CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Werror -Isrc -Itests
BUILD    := build
HEADERS  := $(wildcard src/*.hpp tests/*.hpp)

# every tests/test_*.cpp is a test, no list to keep up to date
TESTS := $(sort $(patsubst tests/%.cpp,%,$(wildcard tests/test_*.cpp)))

# build and run every test; show a test's output only when it fails; summary last
test: $(TESTS:%=$(BUILD)/%)
	@pass=0; fail=0; \
	for t in $^; do \
	  if ./$$t > $$t.log 2>&1; then \
	    echo "PASS $${t#$(BUILD)/}"; pass=$$((pass + 1)); \
	  else \
	    echo "FAIL $${t#$(BUILD)/}"; cat $$t.log; fail=$$((fail + 1)); \
	  fi; \
	done; \
	echo "$$pass passed, $$fail failed"; \
	[ $$fail -eq 0 ]

$(BUILD)/%: tests/%.cpp $(HEADERS)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -rf $(BUILD)

.PHONY: test clean
