#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "progressbar.h"
#ifdef __linux__ 
#include <sys/ioctl.h>
#include <signal.h>
#elif _WIN32
#include <windows.h>
#else
#error "OS not supported!"
#endif

// Only includes characters that take up space (digits excluded)
#define FORMAT "\x1B[?25l\033[1m\033[32m%d / %d:\033[0m ["
// -1 because of null byte termination my beloved
#define FORMAT_LEN 6

static int term_width = 0;
static int term_height = 0;
static char warning_issued = 0;

int digits(int n) {
    if (n == 0) return 1;
    return floor(log10(abs(n))) + 1;
}

#if _WIN32
int handle_interrupt(int sig) {
    if (sig == CTRL_C_EVENT) {
        puts("\x1B[?25h");
        exit(0);
    }
    return 1; // To satisfy WAPI
}
#else
void handle_interrupt(int sig) {
    puts("\x1B[?25h");
    exit(0);
}
#endif

/*
 * Clears the last and prints the current progress bar
 */
void print_bar(p_iter* it) {
    if (!term_width) {
        if (!warning_issued) {
            warning_issued = 1;
            puts("Term width is 0, disabling printing");
        }
        return;
    }

    int space = term_width - it->cached_digits; // - 1 for the closing bracket
    int done = space * it->iterations / it->length;

    putchar('\r'); // Go to start to repaint the progress bar
    printf(FORMAT, it->iterations, it->length);

    int i;
    // - 1 for the leading >
    for (i = 0; i < done - 1; i++) {
        putchar('=');
    }

    if (done == space)
        putchar('=');
    else
        putchar('>');

    for (i = 0; i < space - done; i++) {
        putchar(' ');
    }

    putchar(']');
    if (done == space){
        puts("\x1B[?25h");
    }
}

/*
 * Creates a new progress bar iterator
 *   data: points to your data
 *   unit_size: size of each unit, for example 4 for int
 *   length: number of members in your array (not the size in bytes)
 *   start: what element to start on
 */
p_iter* p_iter_new(void* data, char unit_size, int length, int start) {
    p_iter* it = malloc(sizeof(p_iter));
    it->unit_size = unit_size;
    it->data = data;
    it->length = length;
    it->iterations = start;
    it->cached_digits = (FORMAT_LEN + 1 + (digits(it->length) * 2));
    print_bar(it);
    return it;
}

/*
 * Creates a new simple iterator that returns the next int
 */
p_iter* p_iter_new_simple(int i) {
    return p_iter_new(NULL, 0, i, 0);
}

/*
 * Creates a new iterator that returns the next char
 */
p_iter* p_iter_new_byte(char* data, int length) {
    return p_iter_new(data, 1, length, 0);
}

/*
 * Creates a new iterator that returns the next char in a string (it must be null byte terminated)
 */
p_iter* p_iter_new_string(char* data) {
    char *end = data;
    while(*(end++)) {}
    return p_iter_new(data, 1, end - data - 1, 0);
}

/*
 * Creates a new iterator that returns the next short
 */
p_iter* p_iter_new_short(short* data, int length) {
    return p_iter_new(data, sizeof(short), length, 0);
}

/*
 * Creates a new iterator that returns the next int
 */
p_iter* p_iter_new_int(int* data, int length) {
    return p_iter_new(data, sizeof(int), length, 0);
}

/*
 * Creates a new iterator that returns the next long
 */
p_iter* p_iter_new_long(long* data, int length) {
    return p_iter_new(data, sizeof(long), length, 0);
}

/*
 * Finalizes the iterator
 */
void p_iter_destroy(p_iter* it) {
    free(it);
}

/*
 * Returns the next element of the sequence or NULL of it's been exhausted
 */
long p_iter_next(p_iter* it) {
    print_bar(it);
    if (it->data && it->unit_size) {
        // When iterating over data
        if (it->iterations < it->length) {
            // Perform pointer arithmetic to get the next element, cast it appropriately
            long nextElement = *((long *)(it->data + (it->iterations++ * it->unit_size)));
            return nextElement;
        } else {
            // End of iteration
            return -1; // Special value indicating end
        }
    } else {
        // When simply counting iterations
        if (it->iterations < it->length) {
            return it->iterations++;
        } else {
            // End of iteration
            return -1; // Special value indicating end
        }
    }
}
/*
 * Initializes the library. Safe to call as many times as you want.
 */
void lib_init() {
#ifdef __unix__ // __linux__ ??
    struct winsize ws;
    ioctl( 0 , TIOCGWINSZ, &ws );
    term_width = ws.ws_col;
    term_height = ws.ws_row;

    signal(SIGINT, handle_interrupt);
#elif _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    term_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    term_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    if (!SetConsoleCtrlHandler(handle_interrupt, TRUE)) {
        puts("\nERROR: Could not set control handler"); 
    }
#endif
}
