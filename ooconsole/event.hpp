#pragma once

namespace OOConsole {
    enum EventType {
        MouseButtonUp,
        MouseButtonDown,
        MouseMove,
        MouseVWheeled,
        MouseHWheeled,
        KeyUp,
        KeyDown,
    };

    struct MouseEvent {
        int x, y;
        int buttonState;
        int wheelAmount;
    };

    struct KeyEvent {
        char key;
    };

    struct Event {
        EventType type;
       
        union {
            MouseEvent mouseEvent;
            KeyEvent keyEvent;
        };
    };
}