CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lm

# List your source files here
SRCS = source/sim.c source/engine.c source/therm.c
# This automatically changes the .c extension to .o for object files
OBJS = $(SRCS:.c=.o)

TARGET = sim

# The default rule when you type just 'make'
all: $(TARGET)

# Links the object files together to create the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Compiles each .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleans up the compiled files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
