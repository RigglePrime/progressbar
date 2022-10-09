# Progress bar

A cross platform progress bar and iterator

## Why?

I don't know, I was bored and wanted to learn

## How?

```c
#include <stdio.h>
#include <stdlib.h>
#include "progressbar.h"

int main(void) {
    lib_init();
    p_iter* i = p_iter_new_simple(100);
    int j = p_iter_next(i);
    do {
        // Do something here
    } while(j = p_iter_next(&i));
    p_iter_destroy(i);
    return 0;
}
```

Or maybe

```c
#include <stdio.h>
#include <stdlib.h>
#include "progressbar.h"

int main(void) {
    lib_init();
    p_iter* i = p_iter_new_string("Hello world!");
    char c;
    while (c = p_iter_next(i)) {
        // Do something here
    }
    p_iter_destroy(i);
    return 0;
}
```

```c
#include <stdio.h>
#include <stdlib.h>
#include "progressbar.h"

int main(void) {
    lib_init();
    int arr[] = {1, 2, 3, 4, 5, 6}
    p_iter* i = p_iter_new_int(arr, sizeof(arr));
    int x;
    while (x = p_iter_next(i)) {
        // Do something here
    }
    p_iter_destroy(i);
    return 0;
}
```

## Compiling

`make`

That's it. You're on your own now.

## Issues

- The bar doesn't update on (my) linux machine sometimes. Work fine on Windows.
- Probably many others
