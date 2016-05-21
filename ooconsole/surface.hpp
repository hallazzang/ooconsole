#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

namespace OOConsole {
    class Surface {
    protected:
        int width, height;
        CHAR_INFO *pixels = nullptr;
        CHAR_INFO transparentKey;
        bool hasTransparentKey = false;

    public:
        Surface(int width, int height);
        virtual ~Surface();

        CHAR_INFO& pixelAt(int x, int y);

        void setTransparentKey(char ch, int attr);

        void setPixelChar(int x, int y, char ch);
        void setPixelAttr(int x, int y, int attr);
        void setPixel(int x, int y, char ch, int attr);

        void fillChar(char ch, int x = 0, int y = 0, int width = -1, int height = -1);
        void fillAttr(int attr, int x = 0, int y = 0, int width = -1, int height = -1);
        void fill(char ch, int attr, int x = 0, int y = 0, int width = -1, int height = -1);

        void drawText(int x, int y, char *format, ...);
        void drawText(int x, int y, int attr, char *format, ...);

        void blit(Surface &surface, int x, int y);
    };
}