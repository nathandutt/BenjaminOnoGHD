CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -I/usr/include/eigen3/ -O3
LDFLAGS :=
SRC := src
BIN := bin
BUILD := build
BIN1 := random
BIN2 := evolve
BIN3 := kinetics

.PHONY: all clean                          # 1. declare phony targets

all: $(BIN)/$(BIN1) $(BIN)/$(BIN2) $(BIN)/$(BIN3)   # 2. make 'all' the default (first rule)

# Create directories
$(BUILD) $(BIN):
	@mkdir -p $@

# Object files — also add per-binary build subdirs
$(BUILD)/$(BIN1)/%.o: $(SRC)/$(BIN1)/%.cpp | $(BUILD)
	@mkdir -p $(dir $@)                    # 3. ensure build/random/ exists
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/$(BIN2)/%.o: $(SRC)/$(BIN2)/%.cpp | $(BUILD)
	@mkdir -p $(dir $@)                    # 3. ensure build/evolve/ exists
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/$(BIN3)/%.o: $(SRC)/$(BIN3)/%.cpp | $(BUILD)
	@mkdir -p $(dir $@)                    # 3. ensure build/kinetics/ exists
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Binaries — add | $(BIN) so the bin/ dir is created before linking
$(BIN)/$(BIN1): $(patsubst $(SRC)/$(BIN1)/%.cpp,$(BUILD)/$(BIN1)/%.o,$(wildcard $(SRC)/$(BIN1)/*.cpp)) | $(BIN)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BIN)/$(BIN2): $(patsubst $(SRC)/$(BIN2)/%.cpp,$(BUILD)/$(BIN2)/%.o,$(wildcard $(SRC)/$(BIN2)/*.cpp)) | $(BIN)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BIN)/$(BIN3): $(patsubst $(SRC)/$(BIN3)/%.cpp,$(BUILD)/$(BIN3)/%.o,$(wildcard $(SRC)/$(BIN3)/*.cpp)) | $(BIN)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD) $(BIN) *.res *.dat

rebuild: clean all
