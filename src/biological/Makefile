BUILD_DIR := $(TOP_BUILD_DIR)/biological

CPPFLAGS := -std=c++11 -MMD -MP -O3

$(BUILD_DIR)/%.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

all: $(BUILD_DIR)/biological_tree_generator

# Tree generator

$(BUILD_DIR)/biological_tree_generator: $(BUILD_DIR)/biological_tree_generator.o
	$(CXX) $+ -o $@ $(LDFLAGS)

MKDIR_P ?= mkdir -p
