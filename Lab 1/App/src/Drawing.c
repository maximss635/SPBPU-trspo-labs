//
// Created by maxim on 20.04.2021.
//

#include "Drawing.h"

void savePNG(uint8_t *image, int32_t width, int32_t height, const char* pathSave) {

#ifdef _DYNAMIC_
	void* dLibZ = dlopen("./libz.so", RTLD_LAZY | RTLD_GLOBAL );
	void* dLibPng = dlopen("./libpng16.so", RTLD_LAZY | RTLD_GLOBAL );
#endif // _DYNAMIC_*/

    /* Init */
    FILE* fileSave = fopen(pathSave, "wb");

    png_structp pImage = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    png_infop pImageInfo = png_create_info_struct(pImage);

    png_init_io(pImage, fileSave);

    png_set_IHDR(pImage, pImageInfo, width, height, 8,
                 PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(pImage, pImageInfo);

    uint8_t *row = (uint8_t*)malloc(width * 4);
    for (int32_t y = 0; y < height; ++y) {
        for (int32_t x = 0; x < width; ++x) {
            row[x * 4 + 0] = 0xc0;
            row[x * 4 + 1] = 0xc0;
            row[x * 4 + 2] = 0xc0;
            row[x * 4 + 3] = image[y * width + x];
        }

        png_write_row(pImage, row);
    }

    /* Deinit */

    free(row);
    png_write_end(pImage, 0);

    png_free_data(pImage, pImageInfo, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&pImage, 0);
    fclose(fileSave);

#ifdef _DYNAMIC_
    dlclose(dLibPng);
    dlclose(dLibZ);
#endif // _DYNAMIC_

    return ;
}

int imageCreate(char* pathFont, char* text, char* pathSave) {

#ifdef _DYNAMIC_
    /*  We shoul load functions */

    void (*FT_Init_FreeType)(FT_Library* );
    void (*FT_New_Face)(FT_Library, const char* , FT_Long, FT_Face*);
    void (*FT_Set_Pixel_Sizes)(FT_Face, FT_UInt , FT_UInt);
    void (*FT_Done_Glyph)(FT_Glyph);
    FT_Error (*FT_Done_Face)(FT_Face);
    FT_Error (*FT_Done_FreeType)(FT_Library);

	void* dLibFreeType = dlopen("./libfreetype.so", RTLD_LAZY);
	if (0 == dLibFreeType) {
	    printf("df\n");
	    return -1;
	}

    *(void **)(&FT_Init_FreeType) = dlsym(dLibFreeType, "FT_Init_FreeType");
    *(void **)(&FT_New_Face) = dlsym(dLibFreeType, "FT_New_Face");
    *(void **)(&FT_Set_Pixel_Sizes) = dlsym(dLibFreeType, "FT_Set_Pixel_Sizes");
    *(void **)(&FT_Done_Glyph) = dlsym(dLibFreeType, "FT_Done_Glyph");
    *(void **)(&FT_Done_Face) = dlsym(dLibFreeType, "FT_Done_Face");
    *(void **)(&FT_Done_FreeType) = dlsym(dLibFreeType, "FT_Done_FreeType");
#endif // _DYNAMIC_

    /* Library init */
    FT_Library dLibrary = 0;
    FT_Init_FreeType(&dLibrary);

    FT_Face dFace = 0;
    FT_New_Face(dLibrary, pathFont, 0, &dFace);

    FT_Set_Pixel_Sizes(dFace, 100, 0);

    size_t numSymbols = strlen(text);
    struct SymbolModel* symbols = (struct SymbolModel*)malloc(numSymbols * sizeof(struct SymbolModel));

    int32_t left = INT_MAX;
    int32_t top = INT_MAX;
    int32_t bottom = INT_MIN;

    int32_t posX = 0;
    int symCur, symPrev = 0;
    FT_Glyph glyph;

    printf("Creating image...\n");
    for (size_t i = 0; i < numSymbols; ++i) {
        symCur = text[i];

        glyph = getGlyph(dFace, symCur);
        if (0 == glyph) {
            continue;
        }

        if (symPrev) {
            posX += getKerning(dFace, symPrev, symCur);
        }
        symPrev = symCur;

        struct SymbolModel *symb = &(symbols[i]);
        FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph) glyph;
        symb->posX = (posX >> 6) + bitmapGlyph->left;
        symb->posY = -bitmapGlyph->top;
        symb->width = bitmapGlyph->bitmap.width;
        symb->height = bitmapGlyph->bitmap.rows;
        symb->glyph = glyph;

        posX += glyph->advance.x >> 10;

        left = MIN(left, symb->posX);
        top = MIN(top, symb->posY);
        bottom = MAX(bottom, symb->posY + symb->height);
    }

    for (size_t i = 0; i < numSymbols; ++i) {
        symbols[i].posX -= left;
    }

    const struct SymbolModel *lastSymbol = &(symbols[numSymbols - 1]);

    const int32_t imageW = lastSymbol->posX + lastSymbol->width;
    const int32_t imageH = bottom - top;

    uint8_t *image = (uint8_t*)malloc(imageW * imageH);

    for (size_t i = 0; i < numSymbols; ++i) {
        const struct SymbolModel *symb = symbols + i;

        FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph) symb->glyph;
        FT_Bitmap bitmap = bitmapGlyph->bitmap;

        for (int32_t srcY = 0; srcY < symb->height; ++srcY) {
            const int32_t dstY = symb->posY + srcY - top;

            for (int32_t srcX = 0; srcX < symb->width; ++srcX) {
                const uint8_t c = bitmap.buffer[srcX + srcY * bitmap.pitch];

                if (0 == c) {
                    continue;
                }

                const float a = c / 255.0f;
                const int32_t dstX = symb->posX + srcX;
                uint8_t *dst = image + dstX + dstY * imageW;
                dst[0] = (uint8_t)(a * 255 + (1 - a) * dst[0]);
            }
        }
    }

    printf("Saving image...\n");

    savePNG(image, imageW, imageH, pathSave);
    free(image);

    for (size_t i = 0; i < numSymbols; ++i) {
        FT_Done_Glyph(symbols[i].glyph);
    }

    FT_Done_Face(dFace);
    FT_Done_FreeType(dLibrary);


#ifdef _DYNAMIC_
    dlclose(dLibFreeType);
#endif // _DYNAMIC_

    return 0;
}

FT_Glyph getGlyph(FT_Face face, unsigned int charcode) {
    if (FT_Load_Char(face, charcode, FT_LOAD_RENDER)) {
        printf("Error opening font\n");
        exit(1);
    }

    FT_Glyph glyph = 0;
    FT_Get_Glyph(face->glyph, &glyph);
    return glyph;
}

FT_Pos getKerning(FT_Face face, uint32_t leftCharcode, uint32_t rightCharcode) {
    FT_UInt leftIndex = FT_Get_Char_Index(face, leftCharcode);
    FT_UInt rightIndex = FT_Get_Char_Index(face, rightCharcode);

    FT_Vector delta;
    FT_Get_Kerning(face, leftIndex, rightIndex, FT_KERNING_DEFAULT, &delta);

    return delta.x;

}
