BUILD_DIR := build

all:
	$(MAKE) -C src/digital TOP_BUILD_DIR=../../$(BUILD_DIR)
	$(MAKE) -C src/biological TOP_BUILD_DIR=../../$(BUILD_DIR)

run_tests:
	$(MAKE) -C src/digital run_tests TOP_BUILD_DIR=../../$(BUILD_DIR)	

.PHONY: all clean run_tests

clean:
	$(RM) -r $(BUILD_DIR)
