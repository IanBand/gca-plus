CC = g++

SOURCE  = ./src/*
DEMO  = ./demo/*


GCA_LINKER_FLAGS = -lusb-1.0 -pthread

DEBUG = -g

DEV_FLAGS = -w $(DEBUG)
 
STD = -std=c++17

OUT = ./build/gca_demo

all : 
	$(CC) $(DEV_FLAGS) $(SOURCE) $(GCA_LINKER_FLAGS) $(DEMO) $(STD) -o $(OUT)
