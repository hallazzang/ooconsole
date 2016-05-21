#include "ooconsole/console.hpp"

#include <stdlib.h>

#define MAX_OBJECT_COUNT 512

struct MovableObject {
    bool alive = false;
    int x, y;
};

int main() {
    using namespace OOConsole;

    Console console(80, 40);

    Surface background(80, 40);
    int backgroundX = 0, lastBackgroundMoved = 0;

    int characterX = 1, characterY = 1;

    MovableObject missiles[MAX_OBJECT_COUNT];
    int lastMissilesMoved = 0;

    bool mousePressed = false;
    int lastFired = 0;

    MovableObject enemies[MAX_OBJECT_COUNT];
    int lastEnemyCreated = 0, lastEnemiesMoved = 0;

    int score = 0;

    bool loopFlag = true;

    srand(GetTickCount());

    console.setWindowTitle("Space Ship");
    console.hideCursor();

    console.setColorTable(0, RGB(20, 20, 20));
    console.setColorTable(1, RGB(220, 75, 50));

    background.fill(' ', 0x00);
    for (int i = 0; i < 80; ++i) {
        background.setPixel(rand() % 80, rand() % 40, '.', 0x07);
    }

    while (loopFlag) {
        int now = GetTickCount();

        for each (Event event in console.getEvents()) {
            switch (event.type) {
            case KeyDown:
                if (event.keyEvent.key == ' ') {
                    loopFlag = false;
                }
                break;
            }
        }

        if (now - lastBackgroundMoved >= 75) {
            if (--backgroundX < -80) { backgroundX = 0; }
            lastBackgroundMoved = now;
        }

        console.blit(background, backgroundX, 0);
        console.blit(background, backgroundX + 80, 0);

        console.drawText(30, 19, 0x07, "PRESS SPACE TO START");

        console.update();

        Sleep(5);
    }

    loopFlag = true;
    while (loopFlag) {
        int now = GetTickCount();

        for each (Event event in console.getEvents()) {
            switch (event.type) {
            case MouseMove:
                characterX = event.mouseEvent.x;
                characterY = event.mouseEvent.y;

                if (characterX < 1) {
                    characterX = 1;
                }
                if (characterX > 77) {
                    characterX = 77;
                }
                if (characterY < 1) {
                    characterY = 1;
                }
                if (characterY > 38) {
                    characterY = 38;
                }
                break;
            case MouseButtonDown:
                mousePressed = true;
                break;
            case MouseButtonUp:
                mousePressed = false;
                break;
            case KeyDown:
                if (event.keyEvent.key == 27) {
                    return 0;
                }
            }
        }

        if (mousePressed) {
            if (now - lastFired >= 100) {
                for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
                    if (!missiles[i].alive) {
                        missiles[i].alive = true;
                        missiles[i].x = characterX + 3;
                        missiles[i].y = characterY;
                        break;
                    }
                }

                lastFired = now;
            }
        }

        console.fill(' ', 0);

        if (now - lastBackgroundMoved >= 75) {
            if (--backgroundX < -80) { backgroundX = 0; }
            lastBackgroundMoved = now;
        }

        console.blit(background, backgroundX, 0);
        console.blit(background, backgroundX + 80, 0);

        if (now - lastMissilesMoved >= 25) {
            for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
                if (missiles[i].alive) {
                    if (++missiles[i].x > 79) {
                        missiles[i].alive = false;
                    }
                    else {
                        for (int j = 0; j < MAX_OBJECT_COUNT; ++j) {
                            if (enemies[j].alive) {
                                if ((missiles[i].x == enemies[j].x + 1 || missiles[i].x == enemies[j].x) && missiles[i].y == enemies[j].y) {
                                    missiles[i].alive = false;
                                    enemies[j].alive = false;

                                    score += 10;
                                }
                            }
                        }
                    }
                }
            }

            lastMissilesMoved = now;
        }

        if (now - lastEnemyCreated >= 750) {
            for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
                if (!enemies[i].alive) {
                    enemies[i].alive = true;
                    enemies[i].x = 79;
                    enemies[i].y = rand() % 38 + 1;
                    break;
                }
            }

            lastEnemyCreated = now;
        }

        if (now - lastEnemiesMoved >= 50) {
            for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
                if (enemies[i].alive) {
                    if (--enemies[i].x < 0) {
                        enemies[i].alive = false;
                    }
                    else {
                        if ((enemies[i].y == characterY - 1 && enemies[i].x >= characterX - 1 && enemies[i].x <= characterX + 1) ||
                            (enemies[i].y == characterY && enemies[i].x >= characterX - 1 && enemies[i].x <= characterX + 3) ||
                            (enemies[i].y == characterY + 1 && enemies[i].x >= characterX - 1 && enemies[i].x <= characterX + 1)) {
                            loopFlag = false;
                        }
                    }
                }
            }

            if (!loopFlag) {
                break;
            }

            score += 1;

            lastEnemiesMoved = now;
        }

        for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
            if (enemies[i].alive) {
                console.setPixel(enemies[i].x, enemies[i].y, '<', 0x0e);
            }
            if (missiles[i].alive) {
                console.setPixel(missiles[i].x, missiles[i].y, '-', 0x0b);
            }
        }

        console.drawText(73, 1, 0x0f, "%05d", score);

        console.drawText(characterX - 1, characterY - 1, 0x01, "▲");
        console.drawText(characterX - 1, characterY, 0x01, "◈▶");
        console.drawText(characterX - 1, characterY + 1, 0x01, "▼");

        console.update();

        Sleep(5);
    }


    for (int height = 1; height <= 20; ++height) {
        console.setColorTable(3, RGB(height * 3, height * 3, height * 4));
        console.fill(' ', 0x30, 0, 0, -1, height);
        console.fill(' ', 0x30, 0, 40 - height, -1, height);
        console.update();
        Sleep(20);
    }

    console.drawText(36, 19, 0x37, "YOU LOSE");
    console.drawText(34, 20, 0x37, "SCORE: %05d", score);
    console.update();

    loopFlag = true;
    while (loopFlag) {
        for each (Event event in console.getEvents()) {
            switch (event.type) {
            case KeyDown:
                if (event.keyEvent.key == ' ') {
                    loopFlag = false;
                }
                else if (event.keyEvent.key == 27) {
                    return 0;
                }
                break;
            }
        }
    }

    main();

    return 0;
}