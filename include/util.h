//
// Created by oseias on 04/04/24.
//

#ifndef WMTUPI_UTIL_H
#define WMTUPI_UTIL_H

#endif //WMTUPI_UTIL_H

TScreen* createTScreen(Display* display);

DynamicArray createDynamicArray();

void add(DynamicArray* arr, TWindow item);

void removeItem(DynamicArray* arr, int indexToRemove);