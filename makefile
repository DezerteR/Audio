DIRECTORIES = $(addprefix -I./,$(shell ls -d ./src/*/))
TARGET_NAME = Audio

CXX_FLAGS = -isystem C:\MinGW\include -std=c++1y -O2 -msse2 -mfpmath=sse -g -pipe -I. -I./src $(DIRECTORIES) -DBT_USE_DOUBLE_PRECISION=ON -DUSE_BULLET
# CXX_FLAGS = -isystem C:\MinGW\include -std=c++1y -O3 -msse2 -mfpmath=sse  -pipe -I. -I./src $(DIRECTORIES) -DBT_USE_DOUBLE_PRECISION=ON -DUSE_BULLET
# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
ADDITIONAL_FLAGS = \
-Werror=return-type
# -Wunused-function \
# -Wswitch-enum \

CXX = C:\MinGw\bin\g++.exe
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

clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d
	rm $(BIN)/$(TARGET_NAME).exe

run: $(BIN)/$(TARGET_NAME)
	(cd $(BIN) && ./$(TARGET_NAME).exe)

debug: $(BIN)/$(TARGET_NAME)
	(cd $(BIN) && gdb $(TARGET_NAME).exe)

.PHONY: clean
