//
// Created by maxim on 20.04.2021.
//

#ifndef WORK_DRAWING_H
#define WORK_DRAWING_H

#define TEXT_MAX 25

#ifdef _BLOB_
    #include "BlobWrappers.h"
    #define MAX(x,y) (((x)<(y))?(y):(x))
    #define MIN(x,y) (((x)<(y))?(x):(y))
#else
    #include "ft2build.h"
    #include FT_FREETYPE_H
    #include FT_GLYPH_H
    #include "png.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>

    #define MAX fmax
    #define MIN fmin
#endif // _BLOB_

#ifdef _DYNAMIC_
    #include <dlfcn.h>
#endif // _DYNAMIC_

#include <stdint.h>

struct SymbolModel {
    int32_t posX;
    int32_t posY;
    int32_t width;
    int32_t height;
    FT_Glyph glyph;
};

void savePNG(uint8_t *image, int32_t width, int32_t height, const char* pathSave);

int imageCreate(char* pathFont, char* text, char* pathSave);

FT_Glyph getGlyph(FT_Face face, unsigned int charcode);

FT_Pos getKerning(FT_Face face, uint32_t leftCharcode, uint32_t rightCharcode);

#endif //WORK_DRAWING_H
