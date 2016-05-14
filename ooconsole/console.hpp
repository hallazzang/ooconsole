#pragma once

#include "surface.hpp"

class Console : public Surface {
private:
    HANDLE outputHandle = nullptr;

public:
    Console(int width, int height, bool adjustWindowSize = true);
    virtual ~Console();

    void setColorTable(int index, COLORREF color);

    void showCursor(int size = -1);
    void hideCursor();

    void setWindowSize(int width, int height);
    void setWindowTitle(char *title);

    void update();
};