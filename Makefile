# Our compiler
CC=gcc
# Folders
LIB_DIR=./lib/
INC_DIR=./include/
BIN_DIR=./bin/
SRC_DIR=./src/
TEST_DIR=./tests/
# Flags
CFLAGS=-I$(LIB_DIR) -I$(INC_DIR)
DBFLAGS=-ggdb3 -O0 -g -Wall -Werror
RELEASEFLAGS=-O2
TESTFLAG=-D TEST
FLAGS=

TEST=$(wildcard $(TEST_DIR)*.c)
SRC=$(wildcard $(SRC_DIR)*.c)
OBJ=$(addprefix $(BIN_DIR),$(notdir $(SRC:.c=.o)))
TEST_OBJ=$(addprefix $(BIN_DIR),$(notdir $(TEST:.c=.o)))
EXE=./bin/main
TEST_EXE=./bin/tests

# Default we build release mode
all: release

# Release flags and stuff
release: FLAGS += $(RELEASEFLAGS)
release: OBJ += ./bin/support.o
release:
	@echo "Release Build"
release: executable

# Debug flags and stuff
debug: FLAGS += $(DBFLAGS)
debug: OBJ += ./bin/support.o
debug:
	@echo "Debug Build"
debug: executable

# General main generator
executable: $(OBJ)
	@echo "Linking" $(EXE)
	@$(CC) -o $(EXE) $(OBJ)

# Compiles the .c into .o for the source
./bin/%.o: ./src/%.c
	@mkdir -p $(BIN_DIR)
	@echo "Compiling" $@ 
	@$(CC) -o $@ -c $< $(CFLAGS) $(FLAGS)

# To use when finishing the assignment
delivery: OBJ += ./bin/support.o
delivery: $(OBJ)
	@echo "Delivery Mode"
	@ar crs $(LIB_DIR)libcthread.a $(OBJ)

# Manage the flags for the test mode
tests: FLAGS += $(DBFLAGS)
tests: FLAGS += $(TESTFLAG)
tests: OBJ += ./bin/support.o
tests: test_process
# Generate the test executable in debug mode
test_process: $(TEST_OBJ) $(OBJ)
	@echo "Linking Tests" $(TEST_EXE)
# Need to delete main.o to avoid multiple mains
	@$(CC) -o $(TEST_EXE) $(TEST_OBJ) $(OBJ)

# Compiles the .c into .o for the tests
./bin/%.o: ./tests/%.c
	@echo "Compiling Test" $@ 
	@mkdir -p $(BIN_DIR)
	@$(CC) -o $@ -c $< $(CFLAGS) $(FLAGS)

# Cleaning rule
clean:
	@echo "Cleaning"
	@mv $(BIN_DIR)/support.o $(BIN_DIR)/support.bkp 
	@rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
	@mv $(BIN_DIR)/support.bkp $(BIN_DIR)/support.o 