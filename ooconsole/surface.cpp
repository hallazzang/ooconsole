#include "Surface.hpp"

#define ISTRANSPARENT(surface, x, y) (surface.hasTransparentKey && \
                                      surface.pixelAt(x, y).Char.AsciiChar == surface.transparentKey.Char.AsciiChar && \
                                      surface.pixelAt(x, y).Attributes == surface.transparentKey.Attributes)

namespace OOConsole {
    Surface::Surface(int width, int height) {
        if (width < 1) {
            width = 1;
        }
        if (height < 1) {
            height = 1;
        }

        Surface::width = width;
        Surface::height = height;

        pixels = new CHAR_INFO[width * height];

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                pixelAt(x, y).Char.AsciiChar = ' ';
                pixelAt(x, y).Attributes = 0;
            }
        }
    }
    Surface::~Surface() {
        delete [] pixels;
    }

    CHAR_INFO& Surface::pixelAt(int x, int y) {
        return pixels[width * y + x];
    }
    void Surface::setTransparentKey(char ch, int attr) {
        transparentKey.Char.AsciiChar = ch;
        transparentKey.Attributes = attr;
        hasTransparentKey = true;
    }

    void Surface::setPixelChar(int x, int y, char ch) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixelAt(x, y).Char.AsciiChar = ch;
        }
    }
    void Surface::setPixelAttr(int x, int y, int attr) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixelAt(x, y).Attributes = attr;
        }
    }
    void Surface::setPixel(int x, int y, char ch, int attr) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixelAt(x, y).Char.AsciiChar = ch;
            pixelAt(x, y).Attributes = attr;
        }
    }
    void Surface::fillChar(char ch, int x, int y, int width, int height) {
        int left, top, right, bottom;

        if (width < 0) {
            width = Surface::width - x;
        }
        if (height < 0) {
            height = Surface::height - y;
        }

        left = min(Surface::width, max(0, x));
        top = min(Surface::height, max(0, y));
        right = min(Surface::width, x + width);
        bottom = min(Surface::height, y + height);

        for (int y = top; y < bottom; ++y) {
            for(int x = left; x < right; ++x) {
                pixelAt(x, y).Char.AsciiChar = ch;
            }
        }
    }
    void Surface::fillAttr(int attr, int x, int y, int width, int height) {
        int left, top, right, bottom;

        if (width < 0) {
            width = Surface::width - x;
        }
        if (height < 0) {
            height = Surface::height - y;
        }

        left = min(Surface::width, max(0, x));
        top = min(Surface::height, max(0, y));
        right = min(Surface::width, x + width);
        bottom = min(Surface::height, y + height);

        for (int y = top; y < bottom; ++y) {
            for (int x = left; x < right; ++x) {
                pixelAt(x, y).Attributes = attr;
            }
        }
    }
    void Surface::fill(char ch, int attr, int x, int y, int width, int height) {
        int left, top, right, bottom;

        if (width < 0) {
            width = Surface::width - x;
        }
        if (height < 0) {
            height = Surface::height - y;
        }

        left = min(Surface::width, max(0, x));
        top = min(Surface::height, max(0, y));
        right = min(Surface::width, x + width);
        bottom = min(Surface::height, y + height);

        for (int y = top; y < bottom; ++y) {
            for (int x = left; x < right; ++x) {
                pixelAt(x, y).Char.AsciiChar = ch;
                pixelAt(x, y).Attributes = attr;
            }
        }
    }
    void Surface::drawText(int x, int y, char *format, ...) {
        va_list args;
        char *buffer;
        int destX = x, destY = y;

        va_start(args, format);
        buffer = new char[vsnprintf(NULL, 0, format, args) + 1];
        vsprintf(buffer, format, args);
        va_end(args);

        for (int i = 0; i < strlen(buffer); ++i, ++x) {
            if (buffer[i] == '\n') {
                x = destX - 1;
                ++y;
            }
            else if (x >= 0 && x < width && y >= 0 && y < height) {
                pixelAt(x, y).Char.AsciiChar = buffer[i];
            }
        }

        delete [] buffer;
    }
    void Surface::drawText(int x, int y, int attr, char *format, ...) {
        va_list args;
        char *buffer;
        int destX = x, destY = y;

        va_start(args, format);
        buffer = new char[vsnprintf(NULL, 0, format, args) + 1];
        vsprintf(buffer, format, args);
        va_end(args);

        for (int i = 0; i < strlen(buffer); ++i, ++x) {
            if (buffer[i] == '\n') {
                x = destX - 1;
                ++y;
            }
            else if (x >= 0 && x < width && y >= 0 && y < height) {
                pixelAt(x, y).Char.AsciiChar = buffer[i];
                pixelAt(x, y).Attributes = attr;
            }
        }

        delete [] buffer;
    }
    void Surface::blit(Surface &surface, int x, int y) {
        int destX = x, destY = y;

        for (int y = 0; y < surface.height; ++y) {
            for (int x = 0; x < surface.width; ++x) {
                if (destX + x >= 0 && destX + x < width && destY + y >= 0 && destY + y < height && !ISTRANSPARENT(surface, x, y)) {
                    pixelAt(destX + x, destY + y).Char.AsciiChar = surface.pixelAt(x, y).Char.AsciiChar;
                    pixelAt(destX + x, destY + y).Attributes = surface.pixelAt(x, y).Attributes;
                }
            }
        }
    }
}