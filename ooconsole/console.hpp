#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

class Console {
private:
    HANDLE outputHandle = nullptr;

    int width, height;
    CHAR_INFO *pixels = nullptr;

    int cursorX, cursorY;
    int textAttr;

public:
    Console(int width, int height, bool adjustWindowSize = true);
    virtual ~Console();

    void setColorTable(int index, COLORREF color);

    void showCursor(int size = -1);
    void hideCursor();

    void setWindowSize(int width, int height);
    void setWindowTitle(char *title);

    void setCursorPos(int x, int y);
    void setTextAttr(int attr);

    void setPixel(int x, int y, char ch);
    void setPixel(int x, int y, int attr);
    void setPixel(int x, int y, char ch, int attr);

    void fill(char ch);
    void fill(int attr);
    void fill(char ch, int attr);

    void print(char *format, ...);
    void printTrans(char *format, ...);
    void update();
};