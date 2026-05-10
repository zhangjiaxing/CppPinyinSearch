CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I.
LDFLAGS  :=

TARGET   := piny
SRCS     := main.cpp dictionary.cpp tone.cpp utils.cpp searcher.cpp
OBJS     := $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
