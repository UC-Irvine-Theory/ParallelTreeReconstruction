BUILD_DIR := $(TOP_BUILD_DIR)/wang_et_al
SRCS := run_wang.cpp oracle.cpp wang.cpp
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
TEST_SRCS := test_oracle.cpp test_wang.cpp

CPPFLAGS := -std=c++17 -MMD -MP -O3

$(BUILD_DIR)/%.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

all: $(BUILD_DIR)/run_wang tests

$(BUILD_DIR)/run_wang: $(OBJS)
	$(CXX) $+ -o $@ $(LDFLAGS)

# Tests

tests: $(BUILD_DIR)/test_oracle $(BUILD_DIR)/test_wang

$(BUILD_DIR)/test_oracle: $(BUILD_DIR)/test_oracle.o $(BUILD_DIR)/oracle.o
	$(CXX) $+ -o $@ $(LDFLAGS)

$(BUILD_DIR)/test_wang: $(BUILD_DIR)/test_wang.o $(BUILD_DIR)/oracle.o $(BUILD_DIR)/wang.o
	$(CXX) $+ -o $@ $(LDFLAGS)

# Run  tests
run_tests: $(BUILD_DIR)/test_oracle $(BUILD_DIR)/test_wang
	$(BUILD_DIR)/test_oracle && $(BUILD_DIR)/test_wang

-include $(DEPS)

MKDIR_P ?= mkdir -p
