# VLDWM Display Server Makefile
# Convenience wrapper around CMake

BUILD_DIR := build
CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Release

.PHONY: all clean debug release install uninstall docs test format

all: release

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

debug: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug ..
	cd $(BUILD_DIR) && make -j$(nproc)

release: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) ..
	cd $(BUILD_DIR) && make -j$(nproc)

install: release
	cd $(BUILD_DIR) && sudo make install

uninstall:
	cd $(BUILD_DIR) && sudo make uninstall

clean:
	rm -rf $(BUILD_DIR)

docs: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	cd $(BUILD_DIR) && make docs

test: debug
	cd $(BUILD_DIR) && make test

format:
	find src include -name "*.c" -o -name "*.h" | xargs clang-format -i

help:
	@echo "Available targets:"
	@echo "  all      - Build release version (default)"
	@echo "  debug    - Build debug version"
	@echo "  release  - Build release version"
	@echo "  install  - Install to system"
	@echo "  uninstall- Remove from system"
	@echo "  clean    - Clean build directory"
	@echo "  docs     - Generate documentation"
	@echo "  test     - Run tests"
	@echo "  format   - Format source code"
	@echo "  help     - Show this help"
