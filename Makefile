.PHONY: test clean testclean default

BASE_DIR=.
BIN_DIR=$(BASE_DIR)/bin
INCLUDE_DIR=$(BASE_DIR)/include
SRC_DIR=$(BASE_DIR)/src
TEST_DIR=$(BASE_DIR)/test

CC=clang++
INCLUDE_OPT=-I"$(INCLUDE_DIR)" -I"$(BASE_DIR)"
LINK_OPT=-lclang
MAIN_FILE=$(SRC_DIR)/main.cpp
SRC_FILES=$(filter-out $(MAIN_FILE), $(wildcard $(SRC_DIR)/*.cpp))
TEST_FILE=$(TEST_DIR)/test.cpp
TEST_COMPILE_OUT_FILE=$(TEST_DIR)/test

default:
	$(CC) $(INCLUDE_OPT) $(LINK_OPT) $(SRC_FILES) $(MAIN_FILE) -o $(BIN_DIR)/vscm

test:
	$(CC) $(INCLUDE_OPT) $(LINK_OPT) $(SRC_FILES) $(TEST_FILE) -o $(TEST_DIR)/test
	$(TEST_COMPILE_OUT_FILE)

clean:
	rm -r $(BIN_DIR)/*

testclean:
	rm $(TEST_COMPILE_OUT_FILE)
