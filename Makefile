BUILD_DIR=build
CONFIG_NAME=Release

.PHONY: all
all: make build

.PHONY: make
make:
	mkdir -p $(BUILD_DIR)
	cmake -D PLATFORM_WINDOWS:BOOL=YES -D CMAKE_CXX_COMPILER=clang++ -G "Ninja" -S . -B $(BUILD_DIR)

.PHONY: build
build:
	cmake --build $(BUILD_DIR) --config $(CONFIG_NAME)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/

