//
// Created by Oseias-Isidoro on 13/04/24.
//

#ifndef WMTUPI_UIUTIL_H
#define WMTUPI_UIUTIL_H

    void drawCircle(Display* display, Window win, GC gc, XColor color, int x, int y, int radio, int fill);

    void drawRectangle(Display* display, Window win, GC gc, XColor color, int x, int y, int width, int height, int fill);

    XFontStruct* getFont(char* font_name, Display* display, GC gc);

    void drawText(Display* display, Window win, GC gc, XFontStruct* font_info, XColor color, char* text, int x, int y);

    Window create_simple_window(Display* display, int width, int height, int x, int y);

    GC create_gc(Display* display, Window win, int reverse_video);

#endif //WMTUPI_UIUTIL_H
