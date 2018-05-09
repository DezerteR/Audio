DIRECTORIES = $(addprefix -I./,$(shell ls -d ./src/*/))
TARGET_NAME = Audio
LIB_NAME = libPMK-audio

CXX_FLAGS = -isystem C:\MinGW\include -std=c++1y -O2 -msse2 -mfpmath=sse -g -pipe -I. -I./src $(DIRECTORIES)
ADDITIONAL_FLAGS = -Werror=return-type

CXX = C:\MinGw\bin\g++.exe
INCLUDE_PATH = C:/MinGw/include/Audio
LIB_PATH = C:/MinGw/lib
FFMPEG = C:\ffmpeg\bin\ffmpeg.exe
SRC = ./src
BIN = ./bin
OBJ_DIR = ./obj

LIBS = -lboost_system -lboost_filesystem -lboost_thread -lboost_date_time -lOpenAL32 -lvorbisfile  -lvorbis -logg -lvorbisenc -lFLAC

TARGETS = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)

OBJS = $(TARGETS:%.cpp=$(OBJ_DIR)/%.o)

DEP = $(OBJS:%.o=%.d)

$(BIN)/$(TARGET_NAME): $(OBJS)
	@echo "Linking: $@"
	@$(CXX) $^ -o $@ $(LIBS)

-include $(DEP)

$(OBJ_DIR)/%.o : %.cpp
	@echo "Compiling: $< "
	@mkdir -p $(@D)
	@$(CXX) $(CXX_FLAGS) $(ADDITIONAL_FLAGS) -MMD -c $< -o $@


$(BIN)/$(LIB_NAME).a: $(OBJS)
	@echo "Library: $(LIB_NAME) "
	ar rcsv $@ $^

clean:
	rm -rf $(OBJ_DIR)
	rm $(BIN)/$(TARGET_NAME).exe

run: $(BIN)/$(TARGET_NAME)
	(cd $(BIN) && ./$(TARGET_NAME).exe)

debug: $(BIN)/$(TARGET_NAME)
	(cd $(BIN) && gdb $(TARGET_NAME).exe -ex=run)

install: $(BIN)/$(LIB_NAME).a
	@echo "installing"
	cp $(BIN)/$(LIB_NAME).a $(LIB_PATH)
	cp ./src/Audio/AudioPlayer.hpp $(INCLUDE_PATH)
	cp ./src/Audio/AudioPlayerUtils.hpp $(INCLUDE_PATH)


.PHONY: clean
