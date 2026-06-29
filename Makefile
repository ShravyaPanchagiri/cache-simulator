# Compiler settings -Using C++17 standard with all optimizations warnings turned on
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3

# Target binary name
TARGET = cache_sim

# Build rules
all: $(TARGET)

$(TARGET): src/main.cpp
		$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET)

clean:
		rm -f $(TARGET)