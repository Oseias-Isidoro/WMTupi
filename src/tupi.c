#include <stdio.h>
#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/Xatom.h>

#include "../include/wm_structs.h"
#include "../include/util.h"
#include "../include/twindow.h"

#define MASTER_KEY        Mod4Mask
#define TERMINAL_OPEN_KEY XK_Return
#define TAB               XK_Tab

WMT wmt;

DynamicArray window_array;

int current_window = 0;

int handle_x_error(Display* display, XErrorEvent* e);

void tupi_init()
{
    wmt.display = XOpenDisplay(NULL);

    if (wmt.display  == NULL) {
        fprintf(stderr, ": cannot connect to X server \n");
        exit(1);
    }

    wmt.screen = createTScreen(wmt.display);
    wmt.run = 1;

    XSelectInput(wmt.display, wmt.screen->win_root, SubstructureRedirectMask | SubstructureNotifyMask |  KeyPressMask | KeyReleaseMask);

    XFlush(wmt.display);

    XSetWindowAttributes attributes;
    attributes.event_mask = ButtonPress | KeyPress | SubstructureRedirectMask | SubstructureNotifyMask | PropertyChangeMask | KeyPressMask | KeyReleaseMask;
    XChangeWindowAttributes(wmt.display,wmt.screen->win_root, CWEventMask, &attributes);

    XSetErrorHandler(handle_x_error);
}

void mapRequestEventHandler(XMapRequestEvent event)
{
    Window win = event.window;

    XSelectInput(wmt.display, win, SubstructureRedirectMask | SubstructureNotifyMask | EnterWindowMask |  KeyPressMask | KeyReleaseMask);

    TWindow* tWindow = createTWindow(&wmt, (Window*) win, 3);

    resizeTWindow(&wmt, tWindow, wmt.screen->width, wmt.screen->height, 5);
    moveTWindow(&wmt, tWindow, 0, 0);
    mapTWindow(&wmt, tWindow);

    XSetInputFocus(wmt.display, win, RevertToPointerRoot, CurrentTime);

    add(&window_array, *tWindow);

    current_window = window_array.size - 1;
}

void configureEventHandler(XConfigureRequestEvent event)
{

    {
        XWindowChanges changes;
        changes.x = 0;
        changes.y = 0;
        changes.width = event.width;
        changes.height = event.height;
        changes.border_width = event.border_width;
        changes.sibling = event.above;
        changes.stack_mode = event.detail;
        XConfigureWindow(wmt.display, event.window, event.value_mask, &changes);
    }

    for (int i = 0; i < window_array.size; ++i) {
        TWindow tWin = window_array.arr[i];
        Window win = (Window) tWin.window;

        if (win == event.window) {
            {
                XWindowChanges changes;
                changes.x = event.x;
                changes.y = event.y;
                changes.width = event.width;
                changes.height = event.height;
                changes.border_width = event.border_width;
                changes.sibling = event.above;
                changes.stack_mode = event.detail;
                XConfigureWindow(wmt.display, (Window) tWin.frame, event.value_mask, &changes);
            }
            break;
        }
    }
}

void nextWindow()
{
    if (window_array.size) {
        if (current_window == window_array.size - 1) {
            current_window = 0;
            mapTWindow(&wmt, &window_array.arr[current_window]);
            XSetInputFocus(wmt.display, (Window) window_array.arr[current_window].window, RevertToPointerRoot, CurrentTime);
        } else if (current_window < window_array.size - 1) {
            current_window++;
            mapTWindow(&wmt, &window_array.arr[current_window]);
            XSetInputFocus(wmt.display, (Window) window_array.arr[current_window].window, RevertToPointerRoot, CurrentTime);
        } else {
            current_window = 0;
            mapTWindow(&wmt, &window_array.arr[current_window]);
            XSetInputFocus(wmt.display, (Window) window_array.arr[current_window].window, RevertToPointerRoot, CurrentTime);
        }
    }
}

void KeyPressHandler(XKeyEvent event)
{
    if(event.state & MASTER_KEY && event.keycode == XKeysymToKeycode(wmt.display, TERMINAL_OPEN_KEY)) {
        system("alacritty &");
        return;
    }

    if(event.state & MASTER_KEY && event.keycode == XKeysymToKeycode(wmt.display, TAB)) {
        nextWindow();
        return;
    }
}

void unmapNotifyHandler(XUnmapEvent event)
{
    for (int i = 0; i < window_array.size; ++i) {
        TWindow tWin = window_array.arr[i];
        Window win = (Window) tWin.window;

        if (win == event.window) {
            removeItem(&window_array, i);
            unmapTWindow(&wmt, &tWin);
            nextWindow();
            break;
        }
    }
}

void tupi_run()
{
    XEvent e;

    while (wmt.run) {
        XFlush(wmt.display);
        XNextEvent(wmt.display, &e);

        switch (e.type) {
            case UnmapNotify:
                unmapNotifyHandler(e.xunmap);
                break;
            case MapRequest:
                mapRequestEventHandler(e.xmaprequest);
                break;
            case KeyPress:
                KeyPressHandler(e.xkey);
                break;
            case ConfigureRequest:
                configureEventHandler(e.xconfigurerequest);
                break;
        }
    }
}

int main()
{
    window_array = createDynamicArray();

    tupi_init();
    tupi_run();
    XCloseDisplay(wmt.display);
    return 0;
}

int handle_x_error(Display* display, XErrorEvent* e) {
    (void)display;
    char err_msg[1024];
    XGetErrorText(display, e->error_code, err_msg, sizeof(err_msg));
    printf("X Error:\n\tRequest: %i\n\tError Code: %i - %s\n\tResource ID: %i\n",
           e->request_code, e->error_code, err_msg, (int)e->resourceid);
    return 0;
}
