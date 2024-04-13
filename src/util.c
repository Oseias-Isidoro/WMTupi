//
// Created by oseias on 04/04/24.
//

#include <stdlib.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>		/* getenv(), etc. */
#include <unistd.h>		/* sleep(), etc.  */
#include <string.h>
#include <time.h>

#include "../include/wm_structs.h"
#include "../include/util.h"


TScreen* createTScreen(Display* display) {
    int default_screen_index = DefaultScreen(display);

    TScreen *tScreen;
    tScreen = (TScreen *) malloc(sizeof(TScreen));

    tScreen->screen = XScreenOfDisplay(display, default_screen_index);
    tScreen->win_root = RootWindowOfScreen(tScreen->screen);
    tScreen->width = DisplayWidth(display, default_screen_index);
    tScreen->height = DisplayHeight(display, default_screen_index);

    return tScreen;
}

DynamicArray createDynamicArray() {
    DynamicArray list;

    list.size = 0;
    list.arr = (TWindow*) malloc(list.size * sizeof(TWindow));

    return list;
};

void add(DynamicArray* list, TWindow item) {
    list->size = list->size + 1;
    list->arr = (TWindow*) realloc(list->arr, list->size  * sizeof(TWindow));
    list->arr[list->size-1] = item;
}

void removeItem(DynamicArray* list, int indexToRemove) {
    TWindow* temp = malloc((list->size - 1) * sizeof(TWindow)); // allocate an array with a size 1 less than the current one

    memmove(
            temp,
            list->arr,
            (indexToRemove) * sizeof(TWindow)); // copy everything BEFORE the index

    memmove(
            temp+indexToRemove,
            (list->arr)+(indexToRemove+1),
            (list->size - indexToRemove - 1) * sizeof(TWindow)); // copy everything AFTER the index

    free (list->arr);
    list->arr = temp;
    list->size = list->size - 1;
}
