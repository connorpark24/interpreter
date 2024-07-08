# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Directories
SRCDIR = .
BUILDDIR = build
TARGET = interpreter

# Source files
SRC = $(wildcard $(SRCDIR)/frontend/*.cpp $(SRCDIR)/runtime/*.cpp $(SRCDIR)/main.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

# Create build directory if it doesn't exist
$(shell mkdir -p $(BUILDDIR)/frontend $(BUILDDIR)/runtime)

# Build target
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build objects
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean build files
clean:
	rm -rf $(BUILDDIR)

# Phony targets
.PHONY: clean
