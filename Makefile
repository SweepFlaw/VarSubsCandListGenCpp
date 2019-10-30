.PHONY: test clean

BASE_DIR=.
BIN_DIR=$(BASE_DIR)/bin
INCLUDE_DIR=$(BASE_DIR)/include
SRC_DIR=$(BASE_DIR)/src
TEST_DIR=$(BASE_DIR)/test

CC=clang++
INCLUDE_OPT=-I"$(INCLUDE_DIR)" -I"$(BASE_DIR)"
LINK_OPT=-lclang
SRC_FILES=$(SRC_DIR)/*.cpp
TEST_FILE=$(TEST_DIR)/test.cpp

test:
	$(CC) $(INCLUDE_OPT) $(LINK_OPT) $(SRC_FILES) $(TEST_FILE) -o $(TEST_DIR)/test
	$(TEST_DIR)/test

clean:
	rm -r $(BIN_DIR)/*
