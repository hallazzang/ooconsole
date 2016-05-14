#include "console.hpp"

Console::Console(int width, int height, bool adjustWindowSize) : Surface(width, height) {
    outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (adjustWindowSize) {
        setWindowSize(Console::width, Console::height);
    }
}
Console::~Console() {
}

void Console::setColorTable(int index, COLORREF color) {
    if (index >= 0 && index <= 0xf) {
        CONSOLE_SCREEN_BUFFER_INFOEX screenBufferInfo;

        screenBufferInfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

        GetConsoleScreenBufferInfoEx(outputHandle, &screenBufferInfo);

        screenBufferInfo.ColorTable[index] = color;
        ++screenBufferInfo.srWindow.Right;
        ++screenBufferInfo.srWindow.Bottom;

        SetConsoleScreenBufferInfoEx(outputHandle, &screenBufferInfo);
    }
}
void Console::showCursor(int size) {
    CONSOLE_CURSOR_INFO cursorInfo;

    if (size == -1) {
        GetConsoleCursorInfo(outputHandle, &cursorInfo);
    }
    else {
        if (size < 0) {
            size = 0;
        }
        else if (size > 100) {
            size = 100;
        }

        cursorInfo.dwSize = size;
    }

    cursorInfo.bVisible = TRUE;

    SetConsoleCursorInfo(outputHandle, &cursorInfo);
}
void Console::hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(outputHandle, &cursorInfo);

    cursorInfo.bVisible = FALSE;

    SetConsoleCursorInfo(outputHandle, &cursorInfo);
}
void Console::setWindowSize(int width, int height) {
    CONSOLE_SCREEN_BUFFER_INFOEX screenBufferInfo;

    if (width < 15) {
        width = 15;
    }
    if (height < 1) {
        height = 1;
    }

    screenBufferInfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    GetConsoleScreenBufferInfoEx(outputHandle, &screenBufferInfo);

    screenBufferInfo.dwSize.X = width;
    screenBufferInfo.dwSize.Y = height;

    screenBufferInfo.dwMaximumWindowSize.X = width;
    screenBufferInfo.dwMaximumWindowSize.Y = height;

    screenBufferInfo.srWindow.Right = width;
    screenBufferInfo.srWindow.Bottom = height;

    SetConsoleScreenBufferInfoEx(outputHandle, &screenBufferInfo);
}
void Console::setWindowTitle(char *title) {
    SetConsoleTitleA(title);
}

void Console::update() {
    COORD bufferSize = {width, height}, bufferCoord = {0, 0};
    SMALL_RECT writeRegion = {0, 0, width - 1, height - 1};

    WriteConsoleOutputA(outputHandle, pixels, bufferSize, bufferCoord, &writeRegion);
}