//
// Created by oseias on 04/04/24.
//

#ifndef WMTUPI_UTIL_H
#define WMTUPI_UTIL_H

    Display* getDisplay();

    TScreen* createTScreen(Display* display);

    void grab_input(WMT* wmt, Window win, unsigned int master_key, unsigned int action_key);

    void allocColor(Display* display, XColor* xcolor, char* color_code);

    DynamicArray createDynamicArray();

    void add(DynamicArray* arr, TWindow item);

    void removeItem(DynamicArray* arr, int indexToRemove);

#endif //WMTUPI_UTIL_H
