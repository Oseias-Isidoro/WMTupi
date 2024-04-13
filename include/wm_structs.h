//
// Created by Oseias-Isidoro on 04/04/24.
//

#ifndef WMTUPI_WM_STRUCTS_H
#define WMTUPI_WM_STRUCTS_H

#endif //WMTUPI_WM_STRUCTS_H

typedef struct TupiWindow {
    Window* frame;
    Window* window;
    int border_width;
    int width;
    int height;
} TWindow;

typedef struct {
    int size;
    TWindow *arr;
} DynamicArray;

typedef struct {
    int width;
    int height;
    Screen* screen;
    Window win_root;
    TWindow * windows[5];
    Colormap screen_colormap;
} TScreen;

typedef struct {
    int run;
    Display* display;
    TScreen* screen;
} WMT;