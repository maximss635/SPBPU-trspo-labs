#include "Drawing.h"

#ifdef _BLOB_
extern void** importTable;
#endif // _BLOB_

unsigned long toMs(struct timespec *tm) {
    return (unsigned long)(tm->tv_sec) * 1000
           + (unsigned long)(tm->tv_nsec) / 1000000;
}


/*
 * Usage:   [app_*] [PathFont] [Text] [PathSave]
 * */

#ifdef _BLOB_
int main(int argc, char** argv, void** env) {
    importTable = env;
    printf("Import table: %#llx\n", (unsigned long long)importTable);
#else
int main(int argc, char** argv) {
#endif // _BLOB_

    struct timespec timeStart;
    clock_gettime(CLOCK_REALTIME, &timeStart);

    if (argc != 4) {
        printf("Incorrect arguments\n");
        printf("Usage: [app_*] [PathFont] [Text] [PathSave]\n");

        return 1;
    }

    char* pathFont = argv[1];
    char* text = argv[2];
    char* pathSave = argv[3];

    if (strlen(text) > TEXT_MAX) {
        printf("Too long text (max symbols %d)\n", TEXT_MAX);
        return 1;
    }

    int ret = imageCreate(pathFont, text, pathSave);
    if (ret) {
        printf("Some error!\n");
    }

    struct timespec timeEnd;
    clock_gettime(CLOCK_REALTIME, &timeEnd);

    printf("Time: %lu ms\n", toMs(&timeEnd) - toMs(&timeStart));

    return ret;
}
