CC := g++
SRCS := Main.cpp Server.hpp TaskManager.hpp Client.hpp
OBJS := $(SRCS:.cpp=.o)
TARGET := start

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.cpp
	$(CC) -c $< -o $@
	
Server.o: Server.cpp
	$(CC) -c $< -o $@

clean:
	rm -f Main.o start processes.txt received_process.txt