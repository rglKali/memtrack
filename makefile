# Constants
CC = cc
CFLAGS = 
LFLAGS = -O2 -fPIC

# Directories setup
SRC = src
OBJ = build
INC = include
LIB = lib
TEST = tests

# Get all the sources & objects
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

# Add test sources and objects
TSRCS = $(wildcard $(TEST)/*.c)
TEXES = $(patsubst $(TEST)/%.c, $(TEST)/$(OBJ)/%, $(TSRCS))

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

# Rule to create test executables
$(TEST)/build/%: $(TEST)/%.c $(LIB)/libmemtrack.so
	@mkdir -p $(@D)
	@echo "\033[1;35mBuilding test executable $(notdir $@)\033[0m"
	@$(CC) $< --include $(INC)/memtrack.h -L$(LIB) -Wl,-rpath,$(LIB) -lmemtrack -o $@

# Rule to run all tests
tests: $(TEXES) $(LIB)/libmemtrack.so
	@echo "\033[1;32mRunning all the tests\033[0m"
	@for test in $(TEXES); do \
		echo "\033[1;34mRunning test $$(basename $$test)...\033[0m"; \
		$$test || exit 1; \
	done
	@echo "\033[1;32mAll tests passed successfully.\033[0m"

# Rule to install the library globally in the system
install: lib/libmemtrack.so include/memtrack.h
	@echo "\033[1;32mInstalling the library\033[0m"
	@cp lib/libmemtrack.so /usr/lib/
	@cp include/memtrack.h /usr/include/

# Rule to uninstall the library from the standard paths
uninstall:
	@echo "\033[1;31mUninstalling the library\033[0m"
	@$(RM) /usr/lib/libmemtrack.so
	@$(RM) /usr/include/memtrack.h

# Rule to clean the artifacts
clean:
	@echo "\033[1;33mCleaning build artifacts\033[0m"
	@rm -rf $(OBJ) $(TEST)/build

# Rule to clean library & objects
cleanall:
	@echo "\033[1;33mCleaning shared library & build artifacts\033[0m"
	@rm -rf $(OBJ) $(LIB) $(TEST)/build memtrack.zip

# Rule to generate the zip archive
zip:
	@echo "\033[1;33mCreating the zip archive\033[0m"
	@zip -r memtrack.zip include src tests makefile README.md -x tests/build/*
