#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

/*
 * Iterator struct
 * unit_size should have the size of one unit of your data (4 for int)
 * *data should contain the pointer your data (wow)
 * iterations is the number of iterations left
 * cached_digits is for performance reasons, we only compute it once 
 */
typedef struct PIter {
    char unit_size;
    char *data;
    int length;
    int iterations;
    char cached_digits;
} p_iter;

p_iter* p_iter_new(void* data, char unit_size, int length, int start);
p_iter* p_iter_new_simple(int i);
p_iter* p_iter_new_byte(char* data, int length);
p_iter* p_iter_new_string(char* data);
p_iter* p_iter_new_short(short* data, int length);
p_iter* p_iter_new_int(int* data, int length);
p_iter* p_iter_new_long(long* data, int length);
void p_iter_destroy(p_iter* it);
long p_iter_next(p_iter* self);
void lib_init();
#endif