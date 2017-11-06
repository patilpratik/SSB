APP     := authenticator
APPNAME := $(APP).out #bin/
SOURCES := $(wildcard src/*.cpp src/*.cu src/*.c)
OBJECTS := $(patsubst src%,obj%, $(patsubst %.cu,%.device.o, $(patsubst %.cpp,%.o, $(patsubst %.c,%.o, $(SOURCES)))))

INCLUDEPATH := ./include
ALL_INCLUDEPATH := ../include
INCLUDE := -I$(INCLUDEPATH) -I$(ALL_INCLUDEPATH) 
LIBPATH := ../lib
LIBLINK	:=-L$(LIBPATH)

LIBS    := #-lcryptopp_wrapper -lCLI_interface -lcryptopp -lkey_mutator  #-lcudart -lGL -lglut

DEADCODESTRIP := -Wl,-static -fvtable-gc -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-s
FLAGS    := -O3 -ffast-math -Wall -fopenmp -g #-Werror
CCFLAGS  := $(FLAGS)
CXXFLAGS := $(FLAGS) -std=c++0x $(DEADCODESRIP)

GENCODE_FLAGS := -gencode arch=compute_20,code=sm_20 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35
NVCCFLAGS     := $(GENCODE_FLAGS) --compiler-options -fno-strict-aliasing -lineinfo -use_fast_math -Xptxas -dlcm=cg

CC   := g++ 
CXX  := g++ 
LN   := ln
#CXX  := mpicxx
#NVCC := /usr/local/cuda/bin/nvcc

all: obj $(OBJECTS)
	@echo "<----------------------- making Authenticator -------------------->"
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJECTS) -o $(APPNAME) $(LIBLINK) $(LIBS)

objects:$(OBJECTS)

obj:
	if test -d obj; \
		then echo obj dir exists; \
	else \
		echo Making obj; \
		mkdir obj; \
	fi
	
obj/cmdline.o: src/cmdline.c
	$(CC) -Wno-unused-but-set-variable -c $< -o $@

src/cmdline.c: src/cmdline.ggo
	gengetopt --input=src/cmdline.ggo --output-dir=src --include-getopt

%.o: ../src/%.c
	$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@

%.o: ../src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

%.device.o: ../src/%.cu
	$(NVCC) $(NVCCFLAGS) -c $< -o $@
lib: objects
	@echo "<----------------------- making libfile_splitter.a -------------------->"
	ar cr $(LIBPATH)/lib$(APP).a $(OBJECTS)
.PHONY : include
include:  
	$(LN) -s $(INCLUDEPATH) $(ALL_INCLUDEPATH)/$(APP)

clean: clean_obj clean_lib
	rm -f $(APPNAME)
clean_obj:
	rm -rf obj/*
clean_lib:	
	rm -rf $(LIBPATH)/lib$(APP).a
