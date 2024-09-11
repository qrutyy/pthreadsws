SOURCE = $(file)
OUTPUT = $(basename $(SOURCE))
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -pthread

.PHONY: all clean
all: $(OUTPUT)

# Compile the source file into an executable
$(OUTPUT): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(OUTPUT)

# Clean the build (remove the executable)
clean:
	rm -f $(OUTPUT)
