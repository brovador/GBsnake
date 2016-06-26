ROM_NAME=rom.gb
BIN=./bin
OBJ=./obj
DIST=./dist
SRC=./src
EMULATOR=./emulator



build:
	mkdir -p $(OBJ)
	mkdir -p $(DIST)
	$(BIN)/gbdk-n-compile.sh $(SRC)/main.c -o $(OBJ)/main.rel
	$(BIN)/gbdk-n-link.sh $(OBJ)/main.rel -o $(OBJ)/main.ihx
	$(BIN)/gbdk-n-make-rom.sh $(OBJ)/main.ihx $(DIST)/$(ROM_NAME)

clean:
	rm -rf $(OBJ)
	rm -f $(DIST)/$(ROM_NAME)

run:
	wine $(EMULATOR)/bgb.exe $(DIST)/$(ROM_NAME)
