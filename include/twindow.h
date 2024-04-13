//
// Created by oseias on 05/04/24.
//

#ifndef WMTUPI_TWINDOW_H
#define WMTUPI_TWINDOW_H

    TWindow* createTWindow(WMT* wmt, Window* win, int border_width);

    void     resizeTWindow(WMT* wmt, TWindow* tWin, int width, int height, int border_width);

    void     moveTWindow(WMT* wmt, TWindow* tWin, int x, int y);

    void     mapTWindow(WMT* wmt, TWindow* tWin);

    void     unmapTWindow(WMT* wmt, TWindow* tWin);

    void maximizeTWindow(WMT* wmt, TWindow* tWin);

#endif //WMTUPI_TWINDOW_H
