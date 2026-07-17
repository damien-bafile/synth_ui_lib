build_dir := "build"
project := "synth_ui_lib"

# Show available recipes
default:
    @just --list --unsorted

# Show available recipes
help:
    @just --list --unsorted

####################
# Development
####################

# Configure and build (debug)
build *args:
    cmake -S . -B {{ build_dir }} -DCMAKE_BUILD_TYPE=Debug
    cmake --build {{ build_dir }} -j {{ args }}

# Configure and build (release)
build-release *args:
    cmake -S . -B {{ build_dir }}/release -DCMAKE_BUILD_TYPE=Release
    cmake --build {{ build_dir }}/release -j {{ args }}

# Clean build artifacts
clean:
    rm -rf {{ build_dir }}

# Clean and rebuild (debug)
rebuild *args: clean build

# Verbose build
build-verbose:
    cmake --build {{ build_dir }} -j --verbose

####################
# Testing
####################

# Configure and build tests, then run them
test *args:
    cmake -S . -B {{ build_dir }} -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
    cmake --build {{ build_dir }} -j
    ctest --test-dir {{ build_dir }} --output-on-failure {{ args }}

####################
# Code Quality
####################

# Lint with clang-tidy (default: all sources)
lint *args="src/":
    clang-tidy {{ args }} -p {{ build_dir }} $(if [ -f compile_commands.json ]; then echo ""; else echo "-p {{ build_dir }}"; fi)

# Check formatting with clang-format
format-check:
    find src/ -name '*.cpp' -o -name '*.h' | xargs clang-format --dry-run --Werror

# Format code in-place
format:
    find src/ -name '*.cpp' -o -name '*.h' | xargs clang-format -i

####################
# Information
####################

# Show project info
info:
    @echo "{{ project }} - synth UI library for embedded and desktop"
    @echo "Build dir: {{ build_dir }}"
    @echo "Compiler: $(c++ --version | head -1)"
    @echo ""
    @echo "Targets:"
    @echo "  synth_ui        - shared library"
    @echo "  synth_ui_static - static library"
