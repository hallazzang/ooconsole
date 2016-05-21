#include <conio.h>
#include "ooconsole/console.hpp"

int main() {
    using namespace OOConsole;

    Console console(40, 20);
    int characterX = 0, characterY = 0;
    int backgroundX = 0, backgroundY = 0, backgroundHDelta = 2, backgroundVDelta = 1, backgroundLastMoved = 0;

    console.setWindowTitle("Moving Example");
    console.hideCursor();

    while (1) {
        // Process key input
        if (_kbhit()) {
            switch (_getch()) {
            case 72:
                if (--characterY < 0) {
                    characterY = 0;
                }
                break;
            case 80:
                if (++characterY > 17) {
                    characterY = 17;
                }
                break;
            case 75:
                if (--characterX < 0) {
                    characterX = 0;
                }
                break;
            case 77:
                if (++characterX > 37) {
                    characterX = 37;
                }
                break;
            }
        }

        // Move background (interval: 50ms)
        int now = GetTickCount();

        if (now - backgroundLastMoved >= 50) {
            backgroundX += backgroundHDelta;
            backgroundY += backgroundVDelta;

            if (backgroundX < 0 || backgroundX > 38) {
                if (backgroundX < 0) {
                    backgroundX = 0;
                }
                else {
                    backgroundX = 38;
                }
                backgroundHDelta = -backgroundHDelta;
            }
            if (backgroundY < 0 || backgroundY > 19) {
                if (backgroundY < 0) {
                    backgroundY = 0;
                }
                else {
                    backgroundY = 19;
                }
                backgroundVDelta = -backgroundVDelta;
            }

            backgroundLastMoved = now;
        }

        console.fill(' ', 0x02);

        // Draw background
        console.fill(' ', 0x12, backgroundX, 0, 2, 20);
        console.fill(' ', 0x12, 0, backgroundY, 40, 1);
        console.fill(' ', 0x12, 38 - backgroundX, 0, 2, 20);
        console.fill(' ', 0x12, 0, 19 - backgroundY, 40, 1);

        // Change color tables
        console.setColorTable(1, RGB(backgroundX * 6, 100, 228 - backgroundX * 6));
        console.setColorTable(2, RGB(0, 114 + backgroundX * 3, 0));

        // Draw character (Transparent mode)
        console.drawText(characterX, characterY, " O \n/|)\n/ )");

        console.update();
        Sleep(5);
    }

    return 0;
}