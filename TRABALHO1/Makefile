# Compiler
CXX = g++
CXXFLAGS = -std=c++17

# Source files
SRCS = main.cpp LIGADOR.cpp MONTADOR.cpp pre_processamento.cpp
HEADERS = $(wildcard *.h)
OBJS = $(SRCS:.cpp=.o)

# Directories
BIN_DIR = bin
SRC_DIR = src

# Target executable
TARGET = $(BIN_DIR)/assembler

# Default target
all: $(TARGET)

# Rule for creating the target executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for creating object files
%.o: %.cpp $(HEADERS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create the bin directory if it does not exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Rule for processing .asm files
%.asm:
	./$(TARGET) -p $@
	./$(TARGET) -o $@

# Rule for linking files
link:
	@$(MAKE) $(file1)
	@$(MAKE) $(file2)
	./$(TARGET) -l $(file1:.asm=.e) $(file2:.asm=.e)

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(BIN_DIR)

# Phony targets
.PHONY: all clean link
