# List of modules
# 1)CLI_interface
# 2)cryptopp_wrapper
# 3)key_mutator
# 4)file_splitter
# 5)GUI_interface

APP     := ssb
APPNAME := $(APP).out #bin/
SOURCES := $(wildcard src/*.cpp src/*.cu src/*.c)
OBJECTS := $(patsubst src%,obj%, $(patsubst %.cu,%.device.o, $(patsubst %.cpp,%.o, $(patsubst %.c,%.o, $(SOURCES)))))
#########################################################################
# Commands used
########################################################################
CD	:= cd
CP	:= cp
MAKE	:= make
CC   := g++ 
CXX  := g++ 
LN   := ln
MKDIR	:= mkdir
#CXX  := mpicxx
#NVCC := /usr/local/cuda/bin/nvcc

#INCLUDEPATH := /home/pratik/programing/MS/project/$(APP)/include
ALL_INCLUDEPATH := ./include
#INCLUDE := -I$(INCLUDEPATH) -I$(ALL_INCLUDEPATH) 
INCLUDE := -I$(ALL_INCLUDEPATH) 
LIBPATH := ./lib
LIBLINK	:=-L$(LIBPATH)
DESTDIR := /home/pratik/bin/

##########################################################################
# These are the locations of the modules
##########################################################################
CLI_INTERFACE_DIR	:= ./CLI_interface/
CRYPTOPP_WRAPPER_DIR	:= ./cryptopp_wrapper/
KEY_MUTATOR_DIR		:= ./key_mutator/
FILE_SPLITTER_DIR	:= ./file_splitter/
AUTHENTICATOR_DIR	:= ./authenticator/

GUI_INTERFACE_DIR   := ./GUI_interface/
##########################################################################

LIBS    := -lfile_splitter -lcryptopp_wrapper -lCLI_interface -lkey_mutator -lauthenticator -lcryptopp

DEADCODESTRIP := -Wl, -fvtable-gc -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-s
FLAGS    := -O3 -ffast-math -Wall -fopenmp  -g# -Bstatic#-Werror
CCFLAGS  := $(FLAGS)
CXXFLAGS := $(FLAGS) -std=c++0x $(DEADCODESRIP)
STATICLINK := -lcryptopp #-fno-builtin -static-libstdc++ -static-libgcc -Bstatic -static -lc #-static -lpthread -lgomp -lgcc_s -lm -lgcc_s -lc

GENCODE_FLAGS := -gencode arch=compute_20,code=sm_20 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35
NVCCFLAGS     := $(GENCODE_FLAGS) --compiler-options -fno-strict-aliasing -lineinfo -use_fast_math -Xptxas -dlcm=cg



all: obj make_modules $(OBJECTS)
	@echo "<----------------------- making $(APP) -------------------->"
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJECTS) -o $(APPNAME) $(LIBLINK) $(LIBS) $(STATICLINK)
#	strip --strip-debug $(APPNAME)

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
#lib: 
#	ar cr $(LIBPATH)/lib$(APP).a $(OBJECTS)
.PHONY : include
include:  
	$(LN) -s $(INCLUDEPATH) $(ALL_INCLUDEPATH)/$(APP)
clean: clean_obj clean_lib
	rm -f $(APPNAME)
clean_obj: clean_modules_obj
	rm -rf obj/*
clean_lib: clean_modules_lib

pcl:
	rm -rf pool_base/*
	rm -rf temp/dmesg.txt.out

# This target will build all the modules of the SSB
make_modules: make_file_splitter make_cryptopp_wrapper make_CLI_interface make_key_mutator make_auth make_GUI_interface

################################################################################################
# In here, lib files of the modules will be made.
# for adding any new module, corrosponding target shall be add in this section
################################################################################################
make_file_splitter:
	@echo "----------------------- making file_splitter --------------------"
	$(CD) $(FILE_SPLITTER_DIR) && $(MAKE) lib
make_cryptopp_wrapper:
	@echo "----------------------- making cryptopp_wrapper --------------------"
	$(CD) $(CRYPTOPP_WRAPPER_DIR) && $(MAKE) lib
make_CLI_interface:
	@echo "----------------------- making CLI_interface --------------------"
	$(CD) $(CLI_INTERFACE_DIR) && $(MAKE) lib
make_key_mutator:
	@echo "----------------------- making key_mutator --------------------"
	$(CD) $(KEY_MUTATOR_DIR) && $(MAKE) lib
make_auth:
	@echo "----------------------- making authenticator --------------------"
	$(CD) $(AUTHENTICATOR_DIR) && $(MAKE) lib
make_GUI_interface:
	@echo "----------------------- making GUI_interface --------------------"
	$(CD) $(GUI_INTERFACE_DIR) && qmake ./SecureStorageBox/SecureStorageBox.pro -r -spec linux-g++-64 CONFIG+=debug
	$(CD) $(GUI_INTERFACE_DIR)/build-SecureStorageBox-Desktop-Debug && $(MAKE)
################################################################################################

# This target will clean obj of all the modules

clean_modules_obj: clean_file_splitter_obj clean_cryptopp_wrapper_obj clean_CLI_interface_obj clean_key_mutator_obj clean_auth_obj clean_GUI_interface

# This target will clean lib of all the modules
clean_modules_lib: clean_file_splitter_lib clean_cryptopp_wrapper_lib clean_CLI_interface_lib clean_key_mutator_lib clean_auth_lib


################################################################################################
# In here, obj files of the modules will be cleaned.
# for adding any new module, corrosponding target shall be add in this section
################################################################################################
clean_file_splitter_obj:
	$(CD) $(FILE_SPLITTER_DIR) && $(MAKE) clean_obj
clean_cryptopp_wrapper_obj:
	$(CD) $(CRYPTOPP_WRAPPER_DIR) && $(MAKE) clean_obj
clean_CLI_interface_obj:
	$(CD) $(CLI_INTERFACE_DIR) && $(MAKE) clean_obj
clean_key_mutator_obj:
	$(CD) $(KEY_MUTATOR_DIR) && $(MAKE) clean_obj
clean_auth_obj:
	$(CD) $(AUTHENTICATOR_DIR) && $(MAKE) clean_obj
clean_GUI_interface:
	$(CD) $(GUI_INTERFACE_DIR)/build-SecureStorageBox-Desktop-Debug && $(MAKE) clean
################################################################################################

################################################################################################
# In here, lib files of the modules will be cleaned.
# for adding any new module, corrosponding target shall be add in this section
################################################################################################
clean_file_splitter_lib:
	$(CD) $(FILE_SPLITTER_DIR) && $(MAKE) clean_lib
clean_cryptopp_wrapper_lib:
	$(CD) $(CRYPTOPP_WRAPPER_DIR) && $(MAKE) clean_lib
clean_CLI_interface_lib:
	$(CD) $(CLI_INTERFACE_DIR) && $(MAKE) clean_lib
clean_key_mutator_lib:
	$(CD) $(KEY_MUTATOR_DIR) && $(MAKE) clean_lib
clean_auth_lib:
	$(CD) $(AUTHENTICATOR_DIR) && $(MAKE) clean_lib

################################################################################################
install:
	$(CP) $(APPNAME) $(DESTDIR)/$(APP)

