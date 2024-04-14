//
// Created by Oseias-Isidoro on 05/04/24.
//

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include "../include/wm_structs.h"
#include "../include/util.h"
#include "../include/twindow.h"
#include "../include/config.h"

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

    tWindow->border_width = border_width;
    tWindow->frame = (Window*) win_frame;
    tWindow->window = win;

    XWMHints *sourceHints = XGetWMHints(wmt->display, (Window) win);
    if(sourceHints) {
        XSetWMHints(wmt->display, win_frame, sourceHints);
        XFree(sourceHints);
    }

    XReparentWindow(wmt->display, (Window) tWindow->window, win_frame, border_width, WIN_TOP_BAR_HEIGHT + (border_width));

    //maximizeTWindow(wmt, tWindow);

    return tWindow;
}

void maximizeTWindow(WMT* wmt, TWindow* tWin)
{
    moveTWindow(wmt, tWin, 0, SCREEN_TOP_BAR_HEIGHT);
    resizeTWindow(wmt, tWin, wmt->screen->width, wmt->screen->height - SCREEN_TOP_BAR_HEIGHT, tWin->border_width);
}

void centerTWindow(WMT* wmt, TWindow* tWin)
{
    moveTWindow(
        wmt,
        tWin,
        10,
        10 + SCREEN_TOP_BAR_HEIGHT
    );

    resizeTWindow(
        wmt,
        tWin,
        wmt->screen->width - 20,
        wmt->screen->height - (20 + SCREEN_TOP_BAR_HEIGHT),
        tWin->border_width
    );
}

void moveTWinToRight(WMT* wmt, TWindow* tWin)
{
    moveTWindow(
        wmt,
        tWin,
        (wmt->screen->width /2) + 5,
        SCREEN_TOP_BAR_HEIGHT+ 10
    );

    resizeTWindow(
        wmt,
        tWin,
        (wmt->screen->width / 2) -15,
        wmt->screen->height - (SCREEN_TOP_BAR_HEIGHT + 20),
        tWin->border_width
     );
}

void moveTWinToLeft(WMT* wmt, TWindow* tWin)
{
    moveTWindow(
        wmt,
        tWin,
        10,
        SCREEN_TOP_BAR_HEIGHT + 10
    );

    resizeTWindow(
        wmt,
        tWin,
        (wmt->screen->width / 2) - 15,
        wmt->screen->height - (SCREEN_TOP_BAR_HEIGHT + 20),
        tWin->border_width
    );
}

void resizeTWindow(WMT* wmt, TWindow* tWin, int width, int height, int border_width)
{
    tWin->border_width = border_width;
    tWin->width = width;
    tWin->height = height;

    XResizeWindow(wmt->display, (Window) tWin->frame, width, height);
    XResizeWindow(wmt->display, (Window) tWin->window, width - border_width * 2, height - (WIN_TOP_BAR_HEIGHT + (2 * border_width)));
}

void moveTWindow(WMT* wmt, TWindow* tWin, int x, int y)
{
    XMoveWindow(wmt->display, (Window) tWin->frame, x, y);
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
