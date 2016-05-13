#include <conio.h>
#include "ooconsole/console.hpp"

int main() {
    Console console(40, 20);
    int characterX = 20, characterY = 10;
    int backgroundX = 0, backgroundY = 0, backgroundHDelta = 2, backgroundVDelta = 1, backgroundLastMoved = 0;

    console.setWindowTitle("Moving Example");
    console.setColorTable(1, RGB(174, 198, 207));
    console.setColorTable(2, RGB(204, 84, 96));
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
                if (++characterY > 19) {
                    characterY = 19;
                }
                break;
            case 75:
                if (--characterX < 0) {
                    characterX = 0;
                }
                break;
            case 77:
                if (++characterX > 35) {
                    characterX = 35;
                }
                break;
            }
        }

        console.fill(' ', 0x12);

        // Move background (interval: 30ms)
        int now = GetTickCount();

        if (now - backgroundLastMoved >= 30) {
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

        // Draw background
        console.setCursorPos(backgroundX, 0);
        console.setTextAttr(0x21);
        console.print("  \n  \n  \n  \n  \n  \n  \n  \n  \n  \n  \n  \n  \n  \n  \n  \n  \n  \n  \n  ");

        console.setCursorPos(0, backgroundY);
        console.setTextAttr(0x21);
        console.print("                                        ");

        // Draw character (Transparent mode)
        console.setCursorPos(characterX, characterY);
        console.setTextAttr(0x12);
        console.printTrans("(o_o)");

        console.update();
        Sleep(5);
    }

    return 0;
}