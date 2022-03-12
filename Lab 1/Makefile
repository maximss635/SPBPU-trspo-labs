CC = gcc

OUT_DIR_LIBS = libs_build

FTYPE_PATH = freetype
ZLIB_PATH = zlib
LIBPNG_PATH = libpng
APP_PATH = App
ELFLOADER_PATH = elfloader

FTYPE_STATIC = libfreetype.a
ZLIB_STATIC = libz.a
LIBPNG_STATIC = libpng.a

FTYPE_DYNAMIC = libfreetype.so
ZLIB_DYNAMIC = libz.so
LIBPNG_DYNAMIC = libpng16.so


all:
	make clean
	make static
	make dynamic
	make blob


static:
	@echo "				***** STATIC BUILD *****"

	@echo "				***** Building FREETYPE... (static) *****"
	make $(FTYPE_STATIC)
	@echo "				***** End building FREETYPE ******"

	@echo "				***** Building ZLIB... (static) *****"
	make $(ZLIB_STATIC)
	@echo "				***** End building ZLIB *****"

	@echo "				***** Building LIBPNG... (static) *****"
	make $(LIBPNG_STATIC)
	@echo "				***** End building LIBPNG *****"

	@echo "				***** Building APP... (static) *****"
	make -C $(APP_PATH) static
	@echo "				***** End building APP *****"


dynamic:
	@echo "				***** DYNAMIC BUILD *****"

	@echo "				***** Building FREETYPE... (dynamic) *****"
	make $(FTYPE_DYNAMIC)
	@echo "				***** End building FREETYPE *****"

	@echo "				***** Building ZLIB... (dynamic) *****"
	make $(ZLIB_DYNAMIC)
	@echo "				***** End building ZLIB"

	@echo "				***** Building LIBPNG... (dynamic) *****"
	make $(LIBPNG_DYNAMIC)
	@echo "				***** End building LIBPNG *****"
	
	@echo "				***** Building APP... (dynamic) *****"
	make -C $(APP_PATH) dynamic
	@echo "				***** End building APP *****"


blob:
	@echo "				***** BLOB BUILD *****"

	@echo "				***** Building FREETYPE... (static) *****"
	make $(FTYPE_STATIC)
	@echo "				***** End building FREETYPE ******"

	@echo "				***** Building ZLIB... (static) *****"
	make $(ZLIB_STATIC)
	@echo "				***** End building ZLIB *****"

	@echo "				***** Building LIBPNG... (static) *****"
	make $(LIBPNG_STATIC)
	@echo "				***** End building LIBPNG *****"

	@echo "				***** Building ELFLOADER... *****"
	make -C $(ELFLOADER_PATH)
	@echo "				***** End building APP *****"

	@echo "				***** Building APP... (blob) *****"
	make -C $(APP_PATH) blob
	@echo "				***** End building APP *****"


clean:
	# Clean freetype
	cd $(FTYPE_PATH)/objs && rm -f *.o

	# Clean zlib
	$(MAKE) -C $(ZLIB_PATH) clean

	# Clean libpng
	$(MAKE) -C $(LIBPNG_PATH) clean
	
	# Clean elfloader
	$(MAKE) -C $(ELFLOADER_PATH) clean

	# Clean app
	$(MAKE) -C $(APP_PATH) clean

	rm -rf $(OUT_DIR_LIBS)




# **** Libs compiling ****

# Compiling lib "freetype"
$(FTYPE_STATIC) $(FTYPE_DYNAMIC):
	cd $(FTYPE_PATH) && \
	echo "$(shell pwd)" && \
	#./configure --prefix=$(shell pwd)/$(FTYPE_PATH)/install && \
	$(MAKE) && \
	sudo $(MAKE) install && \
	cd .. && \
	mkdir -p $(OUT_DIR_LIBS) && \
	cp -f $(FTYPE_PATH)/install/lib/$@ $(OUT_DIR_LIBS)/


# Compiling lib "zlib"
$(ZLIB_STATIC) $(ZLIB_DYNAMIC):
	cd $(ZLIB_PATH) && \
	#./configure && \
	$(MAKE) test && \
	cd .. && \
	mkdir -p $(OUT_DIR_LIBS) && \
	cp -f $(ZLIB_PATH)/$@ $(OUT_DIR_LIBS)/


# Compiling (static) lib "libpng"
$(LIBPNG_STATIC):
	cd $(LIBPNG_PATH) && \
	cp -f makefile.gcc Makefile && \
	$(MAKE) && \
	cd .. && \
	cp -f $(LIBPNG_PATH)/$@ $(OUT_DIR_LIBS)/

# Compiling (dynamic) lib "libpng"
$(LIBPNG_DYNAMIC):
	cd $(LIBPNG_PATH) && \
	cp -f makefile.linux Makefile && \
	$(MAKE) && \
	cd .. && \
	cp -f $(LIBPNG_PATH)/$@ $(OUT_DIR_LIBS)/
