#include "console.hpp"

Console::Console(int width, int height, bool adjustWindowSize) {
    outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    cursorX = cursorY = 0;
    textAttr = 0x07;

    if (width < 1) {
        width = 1;
    }
    if (height < 1) {
        height = 1;
    }

    Console::width = width;
    Console::height = height;

    pixels = new CHAR_INFO[width * height];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            pixels[width * y + x].Char.AsciiChar = ' ';
            pixels[width * y + x].Attributes = textAttr;
        }
    }

    if (adjustWindowSize) {
        setWindowSize(width, height);
    }
}
Console::~Console() {
    delete [] pixels;
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

void Console::setCursorPos(int x, int y) {
    cursorX = x;
    cursorY = y;
}
void Console::setTextAttr(int attr) {
    textAttr = attr;
}
void Console::setPixel(int x, int y, char ch) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixels[width * y + x].Char.AsciiChar = ch;
    }
}
void Console::setPixel(int x, int y, int attr) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixels[width * y + x].Attributes = attr;
    }
}
void Console::setPixel(int x, int y, char ch, int attr) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixels[width * y + x].Char.AsciiChar = ch;
        pixels[width * y + x].Attributes = attr;
    }
}
void Console::fill(char ch) {
    for (int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            pixels[width * y + x].Char.AsciiChar = ch;
        }
    }
}
void Console::fill(int attr) {
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            pixels[width * y + x].Attributes = attr;
        }
    }
}
void Console::fill(char ch, int attr) {
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            pixels[width * y + x].Char.AsciiChar = ch;
            pixels[width * y + x].Attributes = attr;
        }
    }
}
void Console::print(char *format, ...) {
    va_list args;
    char buffer[512] = {0, };
    int x = cursorX, y = cursorY;

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    for (int i = 0; i < strlen(buffer); ++i, ++x) {
        if (buffer[i] == '\n') {
            x = cursorX - 1;
            ++y;
        }
        else if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[width * y + x].Char.AsciiChar = buffer[i];
            pixels[width * y + x].Attributes = textAttr;
        }
    }

    cursorX = x;
    cursorY = y;
}
void Console::printTrans(char *format, ...) {
    va_list args;
    char buffer[512] = {0,};
    int x = cursorX, y = cursorY;

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    for(int i = 0; i < strlen(buffer); ++i, ++x) {
        if(buffer[i] == '\n') {
            x = cursorX - 1;
            ++y;
        }
        else if(x >= 0 && x < width && y >= 0 && y < height) {
            pixels[width * y + x].Char.AsciiChar = buffer[i];
        }
    }

    cursorX = x;
    cursorY = y;
}
void Console::update() {
    COORD bufferSize = {width, height}, bufferCoord = {0, 0};
    SMALL_RECT writeRegion = {0, 0, width - 1, height - 1};

    WriteConsoleOutputA(outputHandle, pixels, bufferSize, bufferCoord, &writeRegion);
}