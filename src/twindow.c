//
// Created by oseias on 05/04/24.
//

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include "../include/wm_structs.h"
#include "../include/util.h"
#include "../include/twindow.h"

#define WIN_TOP_BAR_HEIGHT 20

TWindow* createTWindow(WMT* wmt, Window* win, int border_width)
{
    TWindow *tWindow = malloc(sizeof(TWindow));

    XWindowAttributes attribs;
    XGetWindowAttributes(wmt->display, (Window) win, &attribs);

    Window win_frame = XCreateSimpleWindow(
            wmt->display,
            (Window) wmt->screen->win_root,
            attribs.x,
            attribs.y,
            attribs.width + (border_width * 2),
            attribs.height + WIN_TOP_BAR_HEIGHT + (border_width * 2),
            0,
            3,
            0xffffffff);

    XSelectInput(wmt->display, win_frame,  SubstructureRedirectMask | SubstructureNotifyMask | EnterWindowMask | KeyPressMask);

    tWindow->frame = (Window*) win_frame;
    tWindow->window = win;

    XWMHints *sourceHints = XGetWMHints(wmt->display, (Window) win);
    if(sourceHints) {
        XSetWMHints(wmt->display, win_frame, sourceHints);
        XFree(sourceHints);
    }

    resizeTWindow(wmt, tWindow, attribs.width, attribs.height, border_width);

    return tWindow;
}

void resizeTWindow(WMT* wmt, TWindow* tWin, int width, int height, int border_width)
{
    tWin->border_width = border_width;
    tWin->width = width;
    tWin->height = height;

    XResizeWindow(wmt->display, (Window) tWin->frame, width, height);
    XResizeWindow(wmt->display, (Window) tWin->window, width - border_width * 2, height - (WIN_TOP_BAR_HEIGHT + (2 * border_width)));

    XReparentWindow(wmt->display, (Window) tWin->window, (Window) tWin->frame, border_width, WIN_TOP_BAR_HEIGHT + (border_width));
}

void moveTWindow(WMT* wmt, TWindow* tWin, int x, int y)
{
    XMoveWindow(wmt->display, (Window) tWin->frame, x, y);
    XReparentWindow(wmt->display, (Window) tWin->window, (Window) tWin->frame, tWin->border_width, WIN_TOP_BAR_HEIGHT + (tWin->border_width));
}

void mapTWindow(WMT* wmt, TWindow* tWin)
{
    XMapWindow(wmt->display, (Window) tWin->frame);
    XMapWindow(wmt->display, (Window) tWin->window);
    XRaiseWindow(wmt->display, (Window) tWin->frame);
    XRaiseWindow(wmt->display, (Window) tWin->window);
}

void unmapTWindow(WMT* wmt, TWindow* tWin)
{
    XUnmapWindow(wmt->display, (Window) tWin->window);
    XUnmapWindow(wmt->display, (Window) tWin->frame);
}
