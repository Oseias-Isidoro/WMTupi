//
// Created by Oseias-Isidoro on 13/04/24.
//

#include <X11/Xft/Xft.h>

void drawCircle(Display* display, Window win, GC gc, XColor color, int x, int y, int radio, int fill) {
    XSetForeground(display, gc, color.pixel);

    if (fill) {
        XFillArc(display, win, gc, x, y, radio, radio, 0, 360*64);
        return;
    }

    XDrawArc(display, win, gc, x, y, radio, radio, 0, 360*64);
}

void drawRectangle(Display* display, Window win, GC gc, XColor color, int x, int y, int width, int height, int fill) {
    XSetForeground(display, gc, color.pixel);

    if (fill) {
        XFillRectangle(display, win, gc, x, y, width, height);
        return;
    }

    XDrawRectangle(display, win, gc, x, y, width, height);
}

XFontStruct* getFont(char* font_name, Display* display, GC gc) {
    /* check when creating the GC.  */
    unsigned int line_width = 2;                /* line width for the GC.       */
    int line_style = LineSolid;         /* style for lines drawing and  */
    int cap_style = CapButt;            /* style of the line's edje and */
    int join_style = JoinBevel;         /*  joined lines.               */

    /* define the style of lines that will be drawn using this GC. */
    XSetLineAttributes(display, gc, line_width, line_style, cap_style, join_style);
    /* define the fill style for the GC. to be 'solid filling'. */
    XSetFillStyle(display, gc, FillSolid);

    return XLoadQueryFont(display, font_name);
}

void drawText(Display* display, Window win, GC gc, XFontStruct* font_info, XColor color, char* text, int x, int y) {
    /* assign the given font to our GC. */
    XSetForeground(display, gc, color.pixel);
    XSetFont(display, gc, font_info->fid);
    XDrawString(display, win, gc, x, y, text, strlen(text));

    /* find the height of the characters drawn using this font.        */
    //font_height = font_info->ascent + font_info->descent;

    /* draw a "middle of the road" string in the middle of our window. */
    /* find the width, in pixels, of the text that will be drawn using */
    /* the given font.                                                 */
    //string_width = XTextWidth(font_info, text, strlen(text));
}

Window create_simple_window(Display* display, int width, int height, int x, int y)
{
    int screen_num = DefaultScreen(display);
    int win_border_width = 2;
    Window win;

    /* create a simple window, as a direct child of the screen's */
    /* root window. Use the screen's black and white colors as   */
    /* the foreground and background colors of the window,       */
    /* respectively. Place the new window's top-left corner at   */
    /* the given 'x,y' coordinates.                              */
    win = XCreateSimpleWindow(display, RootWindow(display, screen_num),
                              x, y, width, height, win_border_width,
                              BlackPixel(display, screen_num),
                              WhitePixel(display, screen_num));

    /* make the window actually appear on the screen. */
    XMapWindow(display, win);

    /* flush all pending requests to the X server. */
    XFlush(display);

    return win;
}

GC create_gc(Display* display, Window win, int reverse_video)
{
    GC gc;                              /* handle of newly created GC.  */
    unsigned long valuemask = 0;                /* which values in 'values' to  */
    /* check when creating the GC.  */
    XGCValues values;                   /* initial values for the GC.   */
    unsigned int line_width = 2;                /* line width for the GC.       */
    int line_style = LineSolid;         /* style for lines drawing and  */
    int cap_style = CapButt;            /* style of the line's edje and */
    int join_style = JoinBevel;         /*  joined lines.               */
    int screen_num = DefaultScreen(display);

    gc = XCreateGC(display, win, valuemask, &values);
    if (gc < 0) {
        fprintf(stderr, "XCreateGC: \n");
    }

    /* allocate foreground and background colors for this GC. */
    if (reverse_video) {
        XSetForeground(display, gc, WhitePixel(display, screen_num));
        XSetBackground(display, gc, BlackPixel(display, screen_num));
    }
    else {
        XSetForeground(display, gc, BlackPixel(display, screen_num));
        XSetBackground(display, gc, WhitePixel(display, screen_num));
    }

    /* define the style of lines that will be drawn using this GC. */
    XSetLineAttributes(display, gc,
                       line_width, line_style, cap_style, join_style);

    /* define the fill style for the GC. to be 'solid filling'. */
    XSetFillStyle(display, gc, FillSolid);

    return gc;
}

