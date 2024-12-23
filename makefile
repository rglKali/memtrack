# Constants
CC = cc
CFLAGS = 
LFLAGS = -O2 -fPIC

# Directories setup
SRC = src
INC = inc
OBJ = obj
CMD = cmd
BIN = bin
LIB = lib

# Collect the tracks
CMDS = $(wildcard $(CMD)/track_*.c)
BINS = $(patsubst $(CMD)/%.c,$(BIN)/%,$(CMDS))
TRACKS = $(notdir $(BINS))
TESTS = $(wildcard $(CMD)/test_*.c)

# Define the default rule
default: $(LIB)/libmemtrack.so

# Define the rule to produce all executables
all: $(BINS) $(LIB)/libmemtrack.so

# Function to create independent tracks
define generate_simple_rule
$(BIN)/$(1): $(CMD)/$(1).c
	@mkdir -p $$(@D)
	@echo "\033[1;34mCompiling executable $$(notdir $$@)\033[0m"
	@$(CC) $(CFLAGS) $$< -o $$@
endef

# Function to create dependent from mtrack.[c|h|o] tracks
define generate_complex_rule
$(BIN)/$(1): $(CMD)/$(1).c $(INC)/m$(1).h $(OBJ)/m$(1).o
	@mkdir -p $$(@D)
	@echo "\033[1;34mCompiling executable $$(notdir $$@)\033[0m"
	@$(CC) $(CFLAGS) $(CMD)/$(1).c $(OBJ)/m$(1).o -I$(INC) --include $(INC)/m$(1).h -o $$@
endef

# Main function to generate rule based on mtrack.[c] existence
define generate_rule
$(if $(wildcard $(SRC)/m$(1).c), $(eval $(call generate_complex_rule,$(1))), $(eval $(call generate_simple_rule,$(1))))
endef

# Eval all the functions
$(foreach track,$(TRACKS),$(eval $(call generate_rule,$(track))))

# Define a rule to generate objects
$(OBJ)/%.o: $(SRC)/%.c $(INC)/%.h
	@mkdir -p $(@D)
	@echo "\033[1;36mCompiling dependency $(notdir $(basename $@))\033[0m"
	@$(CC) $(LFLAGS) -c $< -I$(INC) -o $@

# Define a rule to build a shared library
$(LIB)/lib%.so: $(SRC)/lib%.c $(INC)/%.h
	@echo "\033[1;35mCompiling shared library $(notdir $@)\033[0m"
	@mkdir -p $(@D) $(OBJ)
	@$(CC) -shared $(LFLAGS) -c $< -o $@

# Rule to install the library globally in the system
install: $(LIB)/libmemtrack.so $(INC)/memtrack.h
	@cp $(LIB)/libmemtrack.so /usr/lib/
	@cp $(INC)/memtrack.h /usr/include/

# Rule to uninstall the library from the standard paths
uninstall:
	@$(RM) /usr/lib/libmemtrack.so
	@$(RM) /usr/include/memtrack.h

# Rule to clean the executable
clean:
	@echo "\033[1;33mCleaning up\033[0m"
	@rm -rf $(OBJ) $(BIN) $(LIB)
