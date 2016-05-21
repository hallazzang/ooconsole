#pragma once

#include <vector>

#include "surface.hpp"
#include "event.hpp"

namespace OOConsole {
    class Console : public Surface {
    private:
        HANDLE inputHandle = nullptr, outputHandle = nullptr;
        int lastButtonState = 0;

    public:
        Console(int width, int height, bool adjustWindowSize = true);
        virtual ~Console();

        void setColorTable(int index, COLORREF color);

        void showCursor(int size = -1);
        void hideCursor();

        void setWindowSize(int width, int height);
        void setWindowTitle(char *title);

        void update();

        std::vector<Event> getEvents(int maxCount = 10);
    };
}