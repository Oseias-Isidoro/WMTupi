//
// Created by Osieas-Isidoro 13/04/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

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
#include "../include/uiutil.h"
#include "../include/config.h"

WMT wmt;
DynamicArray window_array;
XColor black, white, backgroud;
int current_window = 0;

int handle_x_error(Display* display, XErrorEvent* e);
void drawTopBar(Display* display, Window win, GC gc, int width, int height, int screen, XFontStruct* font_info);

TWindow* currentTWindow()
{
    return &window_array.arr[current_window];
}

void allocColors(Display* display)
{
    allocColor(wmt.display, &white, "white");
    allocColor(wmt.display, &black, "black");
    allocColor(wmt.display, &backgroud, "#9f9fdf");
}

void tupi_init()
{
    wmt.display = getDisplay();

    wmt.screen = createTScreen(wmt.display);
    wmt.screen->gc = create_gc(wmt.display, wmt.screen->win_root, 0);
    wmt.screen->font_info = getFont("-*-helvetica-*-r-*-*-18-*-*-*-*-*-*-*", wmt.display, wmt.screen->gc);
    wmt.run = 1;

    allocColors(wmt.display);

    XSync(wmt.display, 0);
    XSetWindowBackground(wmt.display, (Window) wmt.screen->win_root, backgroud.pixel);
    XClearWindow(wmt.display, (Window) wmt.screen->win_root);

    XFlush(wmt.display);

    XSetWindowAttributes attributes;
    attributes.event_mask = ButtonPress | KeyPress | SubstructureRedirectMask | SubstructureNotifyMask | PropertyChangeMask | KeyPressMask | KeyReleaseMask;
    XChangeWindowAttributes(wmt.display,wmt.screen->win_root, CWEventMask, &attributes);

    XSetErrorHandler(handle_x_error);

    grab_input(&wmt, wmt.screen->win_root, MASTER_KEY, TERMINAL_OPEN_KEY);
}

void mapRequestEventHandler(XMapRequestEvent event)
{
    Window win = event.window;

    XSelectInput(wmt.display, win, SubstructureRedirectMask | SubstructureNotifyMask | EnterWindowMask |  KeyPressMask | KeyReleaseMask);

    TWindow* tWindow = createTWindow(&wmt, (Window*) win, 3);

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
    if (!window_array.size) return;

    if (current_window == window_array.size - 1) {
        current_window = 0;
    } else if (current_window < window_array.size - 1) {
        current_window++;
    } else {
        current_window = 0;
    }

    setInputFocusTWin(&wmt, currentTWindow());
}

void KeyPressHandler(XKeyEvent event)
{
    if(event.state & MASTER_KEY && event.keycode == XKeysymToKeycode(wmt.display, TERMINAL_OPEN_KEY)) {
        system("alacritty &");
        return;
    }

    if (event.state & MASTER_KEY && event.keycode == XKeysymToKeycode(wmt.display, TAB)) {
        nextWindow();
        return;
    }

    if (event.state & MASTER_KEY && event.keycode == XKeysymToKeycode(wmt.display, XK_Up)) {
        moveTWinToTop(&wmt, currentTWindow());
        return;
    }

    if (event.state & MASTER_KEY && event.keycode == XKeysymToKeycode(wmt.display, XK_Down)) {
        moveTWinToDown(&wmt, currentTWindow());
        return;
    }

    if (event.state & MASTER_KEY && event.keycode == XKeysymToKeycode(wmt.display, XK_Left)) {
        moveTWinToLeft(&wmt, currentTWindow());
        return;
    }

    if (event.state & MASTER_KEY && event.keycode == XKeysymToKeycode(wmt.display, XK_Right)) {
        moveTWinToRight(&wmt, currentTWindow());
        return;
    }

    if (event.state & MASTER_KEY && event.keycode == XKeysymToKeycode(wmt.display, XK_C)) {
        centerTWindow(&wmt, currentTWindow());
    }

    if(event.state & MASTER_KEY && event.keycode == XKeysymToKeycode(wmt.display, MAXIMIZE_KEY)) {
        maximizeTWindow(&wmt, currentTWindow());
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

void enterNotifyHandler(XEnterWindowEvent event)
{
    for (int i=0; i < window_array.size; i++) {
        TWindow tWin = window_array.arr[i];
        Window win = (Window) tWin.window;
        if (win == event.window) {
            setInputFocusTWin(&wmt, &tWin);
            current_window = i;
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
            case EnterNotify:
                enterNotifyHandler(e.xcrossing);
                break;
        }
    }
}

void* update_ui() {
    while(wmt.run)
    {
        usleep(1000000);
        XFlush(wmt.display);
    
        drawTopBar(
            wmt.display, 
            wmt.screen->win_root, 
            wmt.screen->gc, 
            wmt.screen->width, 
            30, 
            0, 
            wmt.screen->font_info);
    }

    return NULL;
}


int main()
{
    window_array = createDynamicArray();

    XInitThreads();
    tupi_init();

    pthread_t ui_thread;
    pthread_create(&ui_thread, NULL, update_ui, NULL);
    tupi_run();

    XCloseDisplay(wmt.display);
    return 0;
}

void drawTopBar(Display* display, Window win, GC gc, int width, int height, int screen, XFontStruct* font_info)
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    drawRectangle(display, win, gc, black, (width / 2) - 150, 0, 300, SCREEN_TOP_BAR_HEIGHT, 1);
    drawRectangle(display, win, gc, black, (width / 2) - 160, 0, 320, 27, 1);
    drawCircle(display, win, gc, black, (width / 2) - 161, 12, 23, 1);
    drawCircle(display, win, gc, black, (width / 2) + 161 - 24, 12, 23, 1);

    int font_height = font_info->ascent + font_info->descent;
    char* text = asctime (timeinfo);
    drawText(display, win, gc, font_info, white, text, (width / 2) - (XTextWidth(font_info, text, strlen(text)) / 2), (SCREEN_TOP_BAR_HEIGHT / 2) + (font_height / 3) - 2);
}

int handle_x_error(Display* display, XErrorEvent* e) {
    (void)display;
    char err_msg[1024];
    XGetErrorText(display, e->error_code, err_msg, sizeof(err_msg));
    printf("X Error:\n\tRequest: %i\n\tError Code: %i - %s\n\tResource ID: %i\n",
           e->request_code, e->error_code, err_msg, (int)e->resourceid);
    return 0;
}
