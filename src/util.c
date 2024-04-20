//
// Created by Oseias-Isidoro on 04/04/24.
//

#include <stdlib.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../include/wm_structs.h"
#include "../include/config.h"
#include "../include/util.h"


Display* getDisplay()
{
    Display* display = XOpenDisplay(NULL);

    if (display  == NULL) {
        fprintf(stderr, ": cannot connect to X server \n");
        exit(1);
    }

    return display;
}

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

void grab_input(WMT* wmt, Window win, unsigned int master_key, unsigned int action_key)
{
    XGrabKey(
        wmt->display,
        XKeysymToKeycode(wmt->display, action_key),
        master_key, 
        win, 
        0, 
        GrabModeAsync,
        GrabModeAsync);
}

void allocColor(Display* display, XColor* xcolor, char* color_code)
{
    Colormap screen_colormap;
    Status rc;

    screen_colormap = DefaultColormap(display, DefaultScreen(display));

    rc = XAllocNamedColor(display, screen_colormap, color_code, xcolor, xcolor);
    if (rc == 0) {
        fprintf(stderr, "XAllocNamedColor - failed to allocated 'red' color.\n");
        exit(1);
    }
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
