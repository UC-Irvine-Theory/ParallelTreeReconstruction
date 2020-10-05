BUILD_DIR := $(TOP_BUILD_DIR)/afshar_et_al
CPPFLAGS := -std=c++17 -MMD -MP -O3

$(BUILD_DIR)/%.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

all: $(BUILD_DIR)/our_digital_reconstructor

$(BUILD_DIR)/our_digital_reconstructor: $(BUILD_DIR)/our_digital_reconstructor.o
	$(CXX) $+ -o $@ $(LDFLAGS)

MKDIR_P ?= mkdir -p
