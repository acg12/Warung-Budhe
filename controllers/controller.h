#include <string.h>
#include <stdlib.h>
#include "..//models//model.h"



void pressEnter() {
    printf("Press enter to continue...");
    getchar();
}

const char *detectOS()
{
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Other";
    #endif
}

void clear() {
    for (int i = 0; i < 10000; i++)
        puts("");
}