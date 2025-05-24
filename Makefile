CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lncurses

SRCS = main.cpp snake.cpp obstacles.cpp menu.cpp scores.cpp
OBJS = $(SRCS:.cpp=.o)3
TARGET = snake

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean 