#ifndef WORK_BLOBWRAPPERS_H
#define WORK_BLOBWRAPPERS_H

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include "png.h"

void** importTable;

#define png_create_write_struct     ((png_structp(*)(png_const_charp,png_voidp, png_error_ptr, png_error_ptr)) \
                                        (importTable[0]))

#define png_create_info_struct      ((png_infop(*)(png_const_structrp)) \
                                        (importTable[1]))

#define png_init_io                 ((void(*)(png_structrp, png_FILE_p)) \
                                        (importTable[2]))

#define png_free_data               ((void(*)(png_const_structrp, png_inforp, png_uint_32, int)) \
                                        (importTable[3]))

#define png_destroy_write_struct    ((void(*)(png_structpp, png_infopp)) \
                                        (importTable[4]))

#define png_write_row               ((void(*)(png_structrp, png_const_bytep)) \
                                        (importTable[5]))

#define png_write_end               ((void(*)(png_structrp, png_inforp)) \
                                        (importTable[6]))

#define png_set_IHDR                ((void(*)(png_const_structrp, png_inforp, \
                                        png_uint_32, png_uint_32,             \
                                        int, int, int, int, int)) \
                                        (importTable[7]))

#define FT_Get_Kerning              ((FT_Error(*)(FT_Face, FT_UInt, FT_UInt, FT_UInt, struct FT_Vector_*)) \
                                        (importTable[8]))

#define FT_Get_Char_Index           ((FT_UInt(*)(FT_Face, FT_ULong)) \
                                        (importTable[9]))

#define FT_Get_Glyph                ((FT_Error(*)(FT_GlyphSlot, FT_Glyph*)) \
                                        (importTable[10]))

#define FT_Init_FreeType            ((FT_Error(*)(FT_Library*)) \
                                        (importTable[11]))

#define FT_New_Face                 ((void(*)(FT_Library, const char*, FT_Long, FT_Face*)) \
                                        (importTable[12]))

#define FT_Done_Face                ((FT_Error(*)(FT_Face)) \
                                        (importTable[13]))

#define FT_Done_Glyph               ((void(*)(FT_Glyph)) \
                                        (importTable[14]))

#define FT_Done_FreeType            ((FT_Error(*)(FT_Library)) \
                                        (importTable[15]))

#define FT_Set_Pixel_Sizes          ((FT_Error(*)(FT_Face, FT_UInt, FT_UInt)) \
                                        (importTable[16]))

#define FT_Load_Char                ((FT_Error(*)(FT_Face, FT_ULong, FT_Int32)) \
                                        (importTable[17]))

#define fclose                      ((int(*)(FILE*)) \
                                        (importTable[18]))

#define fopen                       ((FILE*(*)(const char *,const char*)) \
                                        (importTable[19]))

#define malloc                      ((void*(*)(size_t)) \
                                        (importTable[20]))

#define free                        ((void(*)(void*)) \
                                        (importTable[21]))

#define puts                        ((int(*)(const char*)) \
                                        (importTable[22]))

#define pow                        ((double(*)(double, double)) \
                                        (importTable[23]))

#endif //WORK_BLOBWRAPPERS_H
