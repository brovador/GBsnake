# Directories
GBDK_DIR = ../gbdk
SRC_DIR = ./src
DIST_DIR = ./dist
OBJ_DIR = ./obj

# Binaries
CC	= ./$(GBDK_DIR)/bin/lcc
EMULATOR = ../tools/bgb/bgb.exe

# Files
ROM_FILE = $(DIST_DIR)/rom.gb
OBJ_FILES = $(OBJ_DIR)/main.o \
			$(OBJ_DIR)/snake-tiles.o \
			$(OBJ_DIR)/snake-map.o

all: $(ROM_FILE)

$(ROM_FILE): $(OBJ_FILES)
	@mkdir -p $(DIST_DIR)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c -o $@ $<

.PHONY : run
run: $(ROM_FILE)
	wine $(EMULATOR) $<

.PHONY : clean
clean:
	@rm -rf $(DIST_DIR)
	@rm -rf $(OBJ_DIR)