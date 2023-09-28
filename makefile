ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
BUILD_DIR = bin/build

.PHONY:clean

all:clean config build install
	
build:
	cmake --build $(BUILD_DIR)

install:
	cmake --install $(BUILD_DIR)

config:
	cmake -S $(ROOT_DIR) -B $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) && mkdir $(BUILD_DIR)