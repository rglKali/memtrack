# Constants
CC = cc
CFLAGS = 
LFLAGS = -O2 -fPIC

# Directories setup
SRC = src
OBJ = build
INC = include
LIB = lib

# Get all the sources & objects
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

# Define a rule to build a shared library (default)
$(LIB)/libmemtrack.so: $(OBJS)
	@echo "\033[1;35mCompiling shared library $(notdir $@)\033[0m"
	@mkdir -p $(@D) $(OBJ)
	@$(CC) -shared $(LFLAGS) $^ -o $@

# Define a rule to generate objects
$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	@echo "\033[1;36mCompiling dependency $(notdir $(basename $@))\033[0m"
	@$(CC) $(LFLAGS) -c $< -I$(SRC) -o $@

# Rule to install the library globally in the system
install: lib/libmemtrack.so include/memtrack.h
	@cp lib/libmemtrack.so /usr/lib/
	@cp include/memtrack.h /usr/include/

# Rule to uninstall the library from the standard paths
uninstall:
	@$(RM) /usr/lib/libmemtrack.so
	@$(RM) /usr/include/memtrack.h

# Rule to clean the artifacts
clean:
	@echo "\033[1;33mCleaning build artifacts\033[0m"
	@rm -rf $(OBJ)

# Rule to clean library & objects
cleanall:
	@echo "\033[1;33mCleaning shared library & build artifacts\033[0m"
	@rm -rf $(OBJ) $(LIB)