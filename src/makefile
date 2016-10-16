DIRECTORIES = $(addprefix -I./,$(shell ls -d */))
TARGET_NAME = AUDIO_DEBUG_3
LIB_NAME = libPMK-audio
OBJ_DIR = ../obj
BIN_DIR = ../bin

CXX = C:/MinGw/bin/g++.exe
INCLUDE_PATH = C:/MinGw/include
LIB_PATH = C:/MinGw/lib

CXX_FLAGS = -isystem C:\MinGW\include -std=c++1y -msse2 -mfpmath=sse  -static -I. -I.. $(DIRECTORIES)

LIBS = -lboost_system -lboost_filesystem -lboost_thread -lOpenAL32 -lvorbisfile  -lvorbis -logg -lvorbisenc -lFLAC

LIBS_TO_INCLUDE = $(wildcard ../libs/*.o)

TARGETS = $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJS = $(TARGETS:%.cpp=$(OBJ_DIR)/%.o)
DEP = $(OBJS:%.o=%.d)

$(BIN_DIR)/$(TARGET_NAME): $(OBJS) ../obj/res.o ../obj/TestSuite.o
	@echo "Linking: $@"
	@$(CXX) $^ -o $@ $(LIBS)

# -------------------------------------------

$(OBJ_DIR)/$(LIB_NAME).a: $(OBJS)
	@echo "Library: $(LIB_NAME) "
	ar rcsv $@ $^  $(LIBS_TO_INCLUDE)


-include $(DEP)

# -------------------------------------------
$(OBJ_DIR)/%.o : %.cpp
	@echo "Compiling: $< "
	@mkdir -p $(@D)
	@$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

# -------------------------------------------

$(OBJ_DIR)/res.o: ../resource.rc ../icon.ico
	windres ../resource.rc ../obj/res.o

unpack:
	ar -x ../libs_a/libboost_filesystem.a
	ar -x ../libs_a/libboost_system.a
	ar -x ../libs_a/libboost_thread.a
	ar -x ../libs_a/libFLAC.a
	ar -x ../libs_a/libvorbis.a
	ar -x ../libs_a/libvorbisenc.a
	ar -x ../libs_a/libvorbisfile.a

install: $(OBJ_DIR)/$(LIB_NAME).a
	@echo "installing"
	cp $(OBJ_DIR)/$(LIB_NAME).a $(LIB_PATH)
	cp ./AudioPlayer.hpp $(INCLUDE_PATH)

deleteLib:
	rm $(OBJ_DIR)/$(LIB_NAME).a

clean:
	rm -rf $(OBJ_DIR)/*
	rm $(BIN_DIR)/$(TARGET_NAME).exe

test: $(BIN_DIR)/$(TARGET_NAME)

run: $(BIN_DIR)/$(TARGET_NAME)
	$(BIN_DIR)/$(TARGET_NAME)

.PHONY: install
.PHONY: clean