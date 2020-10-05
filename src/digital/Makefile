BUILD_DIR := $(TOP_BUILD_DIR)/digital

CPPFLAGS := -std=c++17 -MMD -MP -O3

$(BUILD_DIR)/%.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

all: $(BUILD_DIR)/digital_tree_generator $(BUILD_DIR)/tree_stats
	$(MAKE) -C afshar_et_al TOP_BUILD_DIR=../$(BUILD_DIR)
	$(MAKE) -C wang_et_al TOP_BUILD_DIR=../$(BUILD_DIR)

# Tree generator

$(BUILD_DIR)/digital_tree_generator: $(BUILD_DIR)/digital_tree_generator.o
	$(CXX) $+ -o $@ $(LDFLAGS)

# Tree stats

$(BUILD_DIR)/tree_stats: $(BUILD_DIR)/tree_stats.o
	$(CXX) $+ -o $@ $(LDFLAGS)

run_tests:
	$(MAKE) -C wang_et_al run_tests TOP_BUILD_DIR=../$(BUILD_DIR)

MKDIR_P ?= mkdir -p
