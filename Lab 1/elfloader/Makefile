CC = gcc

SOURCES = elf_loader.c main.c wheelc/list.c
OBJS = elf_loader.o main.o list.o

LIBPNG_INCLUDE_DIR = ../libpng/
FREETYPE_INCLUDE_DIR = ../freetype/include/

LIBPNG_STATIC = ../libs_build/libpng.a
LIBFTYPE_STATIC = ../libs_build/libfreetype.a
ZLIB_STATIC = ../libs_build/libz.a

OUT = elfloader

all:
	$(CC) -c -DOS_LINUX -DARCH_X86_64 -I$(LIBPNG_INCLUDE_DIR) -I$(FREETYPE_INCLUDE_DIR) $(SOURCES)
	$(CC) -Wall $(OBJS) $(LIBPNG_STATIC) $(LIBFTYPE_STATIC) $(ZLIB_STATIC) -lm -ldl -fPIC -pie -o $(OUT)

clean:
	rm -rf *.o wheelc/*.o elfloader
