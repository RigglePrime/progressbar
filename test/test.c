#include <stdio.h>
#include <stdlib.h>
#include "../src/progressbar.h"
#ifdef __linux__ 
#elif _WIN32
#include <windows.h>
#else
#error "OS not supported!"
#endif

#ifdef _WIN32
    #define usleep(X) Sleep(X/1000);
#endif

int main(void) {
    lib_init();
    puts("Starting!");
    p_iter* i = p_iter_new_simple(100);
    int j = p_iter_next(i);
    do {
        usleep(100000);
    } while(j = p_iter_next(i));
    p_iter_destroy(i);
    return 0;
}