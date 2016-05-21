#include "console.hpp"

namespace OOConsole {
    Console::Console(int width, int height, bool adjustWindowSize) : Surface(width, height) {
        DWORD consoleMode;

        inputHandle = GetStdHandle(STD_INPUT_HANDLE);
        outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        GetConsoleMode(inputHandle, &consoleMode);
        SetConsoleMode(inputHandle, consoleMode | ENABLE_MOUSE_INPUT);

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

    std::vector<Event> Console::getEvents(int maxCount) {
        std::vector<Event> events;
        INPUT_RECORD *records;
        DWORD recordsRead;

        if (maxCount < 1) {
            maxCount = 1;
        }

        records = new INPUT_RECORD[maxCount];

        PeekConsoleInputA(inputHandle, records, maxCount, &recordsRead);
        if (recordsRead > 0) {
            ReadConsoleInputA(inputHandle, records, maxCount, &recordsRead);

            for (int i = 0; i < recordsRead; ++i) {
                Event event;
                int changed;

                switch (records[i].EventType) {
                case MOUSE_EVENT:
                    switch (records[i].Event.MouseEvent.dwEventFlags) {
                    case MOUSE_MOVED:
                        event.type = MouseMove;
                        event.mouseEvent.x = records[i].Event.MouseEvent.dwMousePosition.X;
                        event.mouseEvent.y = records[i].Event.MouseEvent.dwMousePosition.Y;
                        event.mouseEvent.buttonState = records[i].Event.MouseEvent.dwButtonState;
                        event.mouseEvent.wheelAmount = 0;
                        break;
                    case DOUBLE_CLICK:
                    case 0:
                        if (changed = records[i].Event.MouseEvent.dwButtonState ^ lastButtonState) {
                            if (records[i].Event.MouseEvent.dwButtonState & changed) {
                                event.type = MouseButtonDown;
                            }
                            else {
                                event.type = MouseButtonUp;
                            }

                            lastButtonState = records[i].Event.MouseEvent.dwButtonState;
                        }

                        event.mouseEvent.x = records[i].Event.MouseEvent.dwMousePosition.X;
                        event.mouseEvent.y = records[i].Event.MouseEvent.dwMousePosition.Y;
                        event.mouseEvent.buttonState = changed;
                        event.mouseEvent.wheelAmount = 0;
                        break;
                    case MOUSE_WHEELED:
                        event.type = MouseVWheeled;
                        event.mouseEvent.x = -1;
                        event.mouseEvent.y = -1;
                        event.mouseEvent.buttonState = 0;
                        event.mouseEvent.wheelAmount = (long)records[i].Event.MouseEvent.dwButtonState >> 16;
                        break;
                    case MOUSE_HWHEELED:
                        event.type = MouseHWheeled;
                        event.mouseEvent.x = -1;
                        event.mouseEvent.y = -1;
                        event.mouseEvent.buttonState = 0;
                        event.mouseEvent.wheelAmount = (long)records[i].Event.MouseEvent.dwButtonState >> 16;
                        break;
                    }

                    events.push_back(event);
                    break;
                case KEY_EVENT:
                    if (records[i].Event.KeyEvent.bKeyDown) {
                        event.type = KeyDown;
                        event.keyEvent.key = records[i].Event.KeyEvent.uChar.AsciiChar;
                    }
                    else {
                        event.type = KeyUp;
                        event.keyEvent.key = records[i].Event.KeyEvent.uChar.AsciiChar;
                    }
                
                    events.push_back(event);
                    break;
                }
            }
        }

        delete [] records;

        return events;
    }
}