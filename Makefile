# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = 

# Target executable
TARGET = game

# Source files
SRCS = main_new.cpp intro_new.cpp end_new.cpp saveprogress_new.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files
HEADERS = player_new.h intro_new.h end_new.h saveprogress_new.h

# Default target
all: $(TARGET)

# Create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Install (if needed)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin (may require sudo)"
	@#sudo cp $(TARGET) /usr/local/bin/

# Run the game
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all     - Build the game (default)"
	@echo "  clean   - Remove all build files"
	@echo "  run     - Build and run the game"
	@echo "  debug   - Build with debug symbols"
	@echo "  help    - Show this help message"

.PHONY: all clean install run debug help