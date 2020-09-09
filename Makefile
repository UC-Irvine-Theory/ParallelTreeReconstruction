opeBUILD_DIR ?= ./build
SRC_DIR ?= ./src

SRCS := $(shell find $(SRC_DIR) -name *.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CPPFLAGS ?= -std=c++17 -MMD -MP -O3

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


all: run_wang tree_generator all_tests
debug: CPPFLAGS += -DDEBUG
# debug: BUILD_DIR := $(BUILD_DIR)/debug
debug: all

# Wang et al

run_wang: $(BUILD_DIR)/run_wang.o $(BUILD_DIR)/wang.o $(BUILD_DIR)/oracle.o
	$(CXX) $+ -o $(BUILD_DIR)/$@ $(LDFLAGS)

# Tree generator

tree_generator: $(BUILD_DIR)/digital_tree_generator.o
	$(CXX) $+ -o $(BUILD_DIR)/$@ $(LDFLAGS)

# Tests

all_tests: test_oracle test_wang

test_oracle: $(BUILD_DIR)/test_oracle.o $(BUILD_DIR)/oracle.o
	$(CXX) $+ -o $(BUILD_DIR)/$@ $(LDFLAGS)

test_wang: $(BUILD_DIR)/test_wang.o $(BUILD_DIR)/oracle.o $(BUILD_DIR)/wang.o
	$(CXX) $+ -o $(BUILD_DIR)/$@ $(LDFLAGS)

# Run  tests
test: test_oracle test_wang
	$(BUILD_DIR)/test_oracle && $(BUILD_DIR)/test_wang

# Other

.PHONY: all clean test

# Delete every file under $(BUILD_DIR) except README.md, .gitignore
clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
