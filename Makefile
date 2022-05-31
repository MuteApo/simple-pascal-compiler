BIN_DIR := bin
PAS_DIR := pas
PAS_SRC := nest.pas

NPROC := $(shell nproc)

CC := clang++
CFLAG := -std=c++11 -Oz -ffunction-sections -fdata-sections
LFLAG := -s -flto=thin -Wl,--gc-sections

.PHONY: all pc as util sim build clean visual compile diagnose run debug test2

all: build

pc:
	@$(MAKE) -C src/pc -j$(NPROC) CC="$(CC)" CFLAG="$(CFLAG)" LFLAG="$(LFLAG)" all

as:
	@$(MAKE) -C src/as -j$(NPROC) CC="$(CC)" CFLAG="$(CFLAG)" LFLAG="$(LFLAG)" all

util:
	@$(MAKE) -C src/util -j$(NPROC) CC="$(CC)" CFLAG="$(CFLAG)" LFLAG="$(LFLAG)" all

sim:
	@$(MAKE) -C src/sim -j$(NPROC) CC="$(CC)" CFLAG="$(CFLAG)" LFLAG="$(LFLAG)" all

build: pc as util sim

clean:
	@$(MAKE) -C src/pc clean
	@$(MAKE) -C src/as clean
	@$(MAKE) -C src/util clean
	@$(MAKE) -C src/sim clean

visual: pc
	$(BIN_DIR)/pc -i $(PAS_DIR)/$(PAS_SRC) -V $(BIN_DIR)/tree_run0.viz $(BIN_DIR)/tree_run1.viz
	dot -Tsvg -o $(BIN_DIR)/tree_run0.svg $(BIN_DIR)/tree_run0.viz
	dot -Tsvg -o $(BIN_DIR)/tree_run1.svg $(BIN_DIR)/tree_run1.viz

compile: pc as util
	$(BIN_DIR)/pc -i $(PAS_DIR)/$(PAS_SRC) -o $(BIN_DIR)/assembly.S
	$(BIN_DIR)/as -i $(BIN_DIR)/assembly.S -o $(BIN_DIR)/target.hex
	$(BIN_DIR)/hex2bin -i $(BIN_DIR)/target.hex -o $(BIN_DIR)/target.bin

diagnose: pc as util
	$(BIN_DIR)/pc -i $(PAS_DIR)/$(PAS_SRC) -o $(BIN_DIR)/assembly.S -s
	$(BIN_DIR)/as -i $(BIN_DIR)/assembly.S -o $(BIN_DIR)/target.hex -s
	$(BIN_DIR)/hex2bin -i $(BIN_DIR)/target.hex -o $(BIN_DIR)/target.bin

run: compile sim
	$(BIN_DIR)/rvsim $(BIN_DIR)/target.bin -s 0xFFFFFF

debug: diagnose sim
	$(BIN_DIR)/rvsim $(BIN_DIR)/target.bin -s 0xFFFFFF -d

test2: build
	@$(MAKE) compile PAS_SRC=mat_mul.pas
	@echo "#! /bin/bash \n $(BIN_DIR)/rvsim $(BIN_DIR)/target.bin -s 0xFFFFFF" > tester/agent.sh
	tester/matrix-multiplication tester/agent.sh
