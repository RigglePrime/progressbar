#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/progressbar.h"

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
