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

$(BUILD)/%: tests/%.cpp $(HEADERS) $(CHIP_SRC)
	@mkdir -p $(BUILD)
	@echo "CXX  $*"
	@$(CXX) $(CXXFLAGS) -o $@ $< $(CHIP_SRC)

# Vitis HLS 2023.2 unified flow, set up by hls/hls_config.cfg; output in build/hls
# csim: C simulation   synth: C to RTL   cosim: RTL simulation against the testbench
# export: package as a Vivado IP
HLS_RUN := cd $(BUILD)/hls && $(XILINX_VITIS)/bin
HLS_CFG := --config ../../hls/hls_config.cfg --work_dir blitnet

$(BUILD)/hls:
	@mkdir -p $@

csim: | $(BUILD)/hls
	$(HLS_RUN)/vitis-run --mode hls --csim $(HLS_CFG)
synth: | $(BUILD)/hls
	$(HLS_RUN)/v++ -c --mode hls $(HLS_CFG)
cosim: synth
	$(HLS_RUN)/vitis-run --mode hls --cosim $(HLS_CFG)
export: synth
	$(HLS_RUN)/vitis-run --mode hls --package $(HLS_CFG)

clean:
	rm -rf $(BUILD)

.PHONY: test clean csim synth cosim export
