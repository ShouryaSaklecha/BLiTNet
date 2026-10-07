XILINX_HLS   ?= /tools/Xilinx/Vitis_HLS/2023.2
XILINX_VITIS ?= /tools/Xilinx/Vitis/2023.2

# C++14: what Vitis HLS 2023.2 compiles with, so make test can't accept code HLS rejects
CXX      := g++
XF_LIBS  := vendor/Vitis_Libraries
XF_INC   := -isystem $(XF_LIBS)/utils/L1/include -isystem $(XF_LIBS)/blas/L1/include/hw \
            -isystem $(XF_LIBS)/solver/L1/include/hw -isystem $(XF_LIBS)/data_mover/L1/include
CXXFLAGS := -std=c++14 -O2 -Wall -Wextra -Werror -Wno-unknown-pragmas \
            -Isrc/chip -Isrc/host -Itests -isystem $(XILINX_HLS)/include $(XF_INC) \
            -DAP_INT_MAX_W=4096 -DBN_SOFT_STREAM
BUILD    := build
HEADERS  := $(wildcard src/*/*.hpp tests/*.hpp)
CHIP_SRC := $(wildcard src/chip/*.cpp)

# every tests/test_*.cpp is a test, no list to keep up to date
TESTS := $(sort $(patsubst tests/%.cpp,%,$(wildcard tests/test_*.cpp)))

# build and run every test; quiet unless something fails (V=1 lists every pass)
test: $(TESTS:%=$(BUILD)/%)
	@pass=0; fail=0; \
	for t in $^; do \
	  if ./$$t > $$t.log 2>&1; then \
	    pass=$$((pass + 1)); if [ -n "$(V)" ]; then echo "PASS $${t#$(BUILD)/}"; fi; \
	  else \
	    echo "FAIL $${t#$(BUILD)/}"; head -n 20 $$t.log; fail=$$((fail + 1)); \
	  fi; \
	done; \
	echo "$$pass passed, $$fail failed"; \
	[ $$fail -eq 0 ]

$(BUILD)/%: tests/%.cpp $(HEADERS) $(CHIP_SRC)
	@mkdir -p $(BUILD)
	@$(CXX) $(CXXFLAGS) -o $@ $< $(CHIP_SRC) 2> $@.err || \
	  { echo "BUILD FAIL $*"; head -n 15 $@.err; exit 1; }

# Vitis HLS 2023.2 via tools/hls.py: prints a short digest, raw logs stay in build/hls,
# identical inputs reuse the cached digest (FORCE=1 reruns). Never read build/hls raw files.
csim synth cosim export:
	@python3 tools/hls.py run $@ $(if $(FORCE),--force)

clean:
	rm -rf $(BUILD)

.PHONY: test clean csim synth cosim export
