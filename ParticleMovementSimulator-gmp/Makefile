# Declaration of variables
CC = gcc
CC_FLAGS = -Wall -std=c11
CC_LIBS = -lgmp -lm -lcsv
 
# File names
EXEC = pms
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
 
# Main target
compile: $(OBJECTS)
	$(CC) $(CC_LIBS) $(CC_FLAGS) $(OBJECTS) -o $(EXEC)
 
# To obtain object files
%.o: %.c
	$(CC) $(CC_LIBS) $(CC_FLAGS) -c $< -o $@
 
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)

