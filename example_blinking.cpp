#include "ooconsole/console.hpp"

int main() {
    using namespace OOConsole;

    Console console(40, 20);
    int colorIndex = 0, colorDelta = 1;

    console.setWindowTitle("Blinking Example");
    console.hideCursor();

    console.fill(' ', 0x10);
    console.update();

    while (1) {
        console.setColorTable(1, RGB(colorIndex * 10, colorIndex * 10, colorIndex * 10));

        colorIndex += colorDelta;

        if (colorIndex <= 0 || colorIndex >= 25) {
            colorDelta = -colorDelta;
        }

        Sleep(30);
    }

    return 0;
}