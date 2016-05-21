#include "ooconsole/console.hpp"
#include "ooconsole/surface.hpp"

int main() {
    using namespace OOConsole;

    Console console(40, 20);
    Surface background(20, 9), foreground(10, 5);
    int colorIndex = 0x0;

    console.setWindowTitle("Surface Example");
    console.hideCursor();

    foreground.fill(' ', 0x90);
    foreground.setTransparentKey(' ', 0x00);
    foreground.setPixel(1, 1, ' ', 0x00);
    foreground.setPixel(8, 1, ' ', 0x00);
    foreground.drawText(2, 3, 0x00, "      ");

    while (1) {
        background.fill(' ', colorIndex * 0x10);

        background.blit(foreground, 5, 2);

        console.blit(background, 10, 5);
        console.update();

        if (++colorIndex > 0x8) {
            colorIndex = 0x0;
        }

        Sleep(50);
    }

    return 0;
}