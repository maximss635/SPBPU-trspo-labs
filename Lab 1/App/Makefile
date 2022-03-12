CC = gcc

FLAGS = -c -Wall -O
BLOB_FLAGS = -fno-stack-protector -fno-exceptions -s -nostdlib -pie -fPIE -fPIC -shared

SRC_DIR = src
OUT_DIR = build
OBJ_DIR = obj

LIBPNG_INCLUDE_DIR = ../libpng/
FREETYPE_INCLUDE_DIR = ../freetype/include/
ZLIB_INCLUDE_DIR = ../zlib/

LIBPNG_STATIC = $(OBJ_DIR)/libpng.a
LIBPNG_DYNAMIC = $(OUT_DIR)/libpng16.so

FREETYPE_STATIC = $(OBJ_DIR)/libfreetype.a
FREETYPE_DYNAMIC = $(OUT_DIR)/libfreetype.so

ZLIB_STATIC = $(OBJ_DIR)/libz.a
ZLIB_DYNAMIC = $(OUT_DIR)/libz.so

SOURCES = $(SRC_DIR)/Drawing.c $(SRC_DIR)/Main.c
OBJS = $(OBJ_DIR)/Drawing.o $(OBJ_DIR)/Main.o


all:
	make clean
	make static
	make dynamic
	make blob


common:
	mkdir -p $(OUT_DIR)
	mkdir -p $(OBJ_DIR)
	cp $(SRC_DIR)/font.ttf $(OUT_DIR)/
	cp run.sh $(OUT_DIR)/run.sh


static:
	make common
	cp ../libs_build/*.a $(OBJ_DIR)/

	# Compile app only to .o files
	$(CC) $(FLAGS) -I$(LIBPNG_INCLUDE_DIR) -I$(FREETYPE_INCLUDE_DIR) $(SOURCES)

	mv Main.o $(OBJ_DIR)/Main.o
	mv Drawing.o $(OBJ_DIR)/Drawing.o

	# Static link with libs
	$(CC) -Wall $(OBJS) $(LIBPNG_STATIC) $(ZLIB_STATIC) $(FREETYPE_STATIC) -lm -o $(OUT_DIR)/app_static

dynamic:
	make common
	cp ../libs_build/*.so $(OUT_DIR)/

	# Compile app only to .o files
	$(CC) $(FLAGS) -I$(LIBPNG_INCLUDE_DIR) -I$(FREETYPE_INCLUDE_DIR) -D_DYNAMIC_ $(SOURCES)

	mv Main.o $(OBJ_DIR)/Main.o
	mv Drawing.o $(OBJ_DIR)/Drawing.o

	# Dynamic link with libs
	$(CC) -Wall $(OBJS) $(LIBPNG_DYNAMIC) $(ZLIB_DYNAMIC) $(FREETYPE_DYNAMIC) -lm -ldl -o $(OUT_DIR)/app_dynamic


blob:
	make common
	cp ../libs_build/*.a $(OBJ_DIR)/

	# Make blob.bin
	$(CC) -I$(LIBPNG_INCLUDE_DIR) -I$(FREETYPE_INCLUDE_DIR) -L$(OBJ_DIR) $(BLOB_FLAGS) \
	-D_BLOB_ -emain $(SOURCES) -lfreetype -lpng -lz -o $(OUT_DIR)/blob.bin

	# Copy elfloader to OUT_DIR
	cp ../elfloader/elfloader $(OUT_DIR)/app_blob

clean:
	rm -rf $(OUT_DIR)
	rm -rf $(OBJ_DIR)
