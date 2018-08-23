INC_DIR :=  ./include/cppjieba ./include/network ./include/utils ./include
SRC_DIR = ./src ./src/utils ./src/network
#EXE_DIR := ./bin
CC := g++
CPPFLAGS := -std=c++11 -g -Wno-deprecated
LIBS := -ljson -lpthread -llog4cpp
INC_FILE := $(addprefix -I, $(INC_DIR))
CPP_FILE :=  $(wildcard ./src/utils/*.cc) $(wildcard ./src/network/*.cc) $(wildcard ./src/*.cc)
#OBJS_HPP :=$(wildcard  $(INC_DIR)/*.hpp) $(wildcard $(INC_DIR)/*.h)
TARGET := rssSearchEngine.exe
$(TARGET):$(CPP_FILE)
	$(CC) $(CPPFLAGS) -o $@ $(CPP_FILE) $(INC_FILE) $(LIBS)

clean:
	rm -rf $(TARGET)
