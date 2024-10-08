OBJS =  ./src/imgui/*.cpp ./src/luasocket/*.c ./src/luasec/*.c ./src/engine.cpp

CC = x86_64-w64-mingw32-g++
EMCC = em++
EMCC_FLAGS = --bind -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2  -sALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s TOTAL_STACK=8MB -sWASM=1 -sMAXIMUM_MEMORY=1gb -s INITIAL_MEMORY=65536000 --shell-file shell_minimal.html -sMAIN_MODULE -s -sERROR_ON_UNDEFINED_SYMBOLS=0


INCLUDE_PATHS = -I./includes -I./includes/SDL3 -I./includes/LUA -I./includes/IMGUI -I./src/luasocket/ -I./src/luasec

LIBRARY_PATHS = -L./lib

COMPILER_FLAGS = -w -Wl,-subsystem,windows -fpermissive

LINKER_FLAGS = -static-libstdc++ -static-libgcc -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -lmingw32 -lSDL3 -lSDL3_image -lSDL3_ttf -llua -lssl -lcrypto -lWs2_32

win32_OBJ_NAME = ./win32/voidGEN
EM_OBJ_NAME = ./html/index.html

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(win32_OBJ_NAME)

win32-core: win32-build-dir
	cp ./pre_init.lua ./win32/
	cp -rv ./core/* ./win32/core

win32-assets: win32-build-dir
	cp -rv ./assets/* ./win32/assets

win32-runtimes: win32-build-dir
	cp -v -p ./runtimes/* ./win32/

win32-build-dir:
	mkdir -v -p ./win32/
	mkdir -v -p ./win32/assets
	mkdir -v -p ./win32/core
	
win32-test:
	cd ./win32/ && ./voidGEN
	
clean:
	rm -rfv ./win32/

html: $(OBJS) | em-core
	em++ -lluaw  $(EMCC_FLAGS) -g -lm $(LIBRARY_PATHS) $(INCLUDE_PATHS)  $(OBJS) --preload-file ./assets@/assets --preload-file ./core@/core --preload-file ./pre_init.lua@/ -o $(EM_OBJ_NAME)
	
em-core:
	mkdir -v -p ./html
	
html-test:
	cd ./html && emrun index.html --no-browser
	