//
// Created by anshu on 11/6/2020.
//

#include "EmulatorControls.h"

struct KeyboardEvent *keyboardLastEvent;
struct KeyboardEvent *keyboardEmulator;
struct MouseEvent *mouseInput;

void EmulatorControls::UpdateMouseInput(const Application &app) {
    auto *input = app.GetSubsystem<Input>();
    IntVector2 mouseMove = input->GetMouseMove();

    if (mouseMove.x_ > -2000000000 && mouseMove.y_ > -2000000000) {
        mouseHist.x_ = mouseHist.x_ + mouseMove.x_;
        mouseHist.y_ = mouseHist.y_ + mouseMove.y_;

        if (mouseHist.x_ >= 640) mouseHist.x_ = 640;
        if (mouseHist.x_ < 0) mouseHist.x_ = 0;
        if (mouseHist.y_ >= 400) mouseHist.y_ = 400;
        if (mouseHist.y_ < 0) mouseHist.y_ = 0;

        mouseInput->mouseX = mouseHist.x_;
        mouseInput->mouseY = mouseHist.y_;

        mouseInput->clicked = input->GetMouseButtonDown(MOUSEB_LEFT);
    }
}

void EmulatorControls::UpdateKeyboardOnEventDown(const Application &app, int key) {
    if (keyboardLastEvent == nullptr) {
        keyboardLastEvent = static_cast<KeyboardEvent *>(malloc(sizeof(KeyboardEvent)));
    } else {
        keyboardLastEvent->_nextVal = static_cast<KeyboardEvent *>(malloc(sizeof(KeyboardEvent)));
        keyboardLastEvent = keyboardLastEvent->_nextVal;
    }

    keyboardLastEvent->value = key;
    keyboardLastEvent->isUp = 0;
    keyboardLastEvent->_nextVal = nullptr;

    if (keyboardEmulator == nullptr)
        keyboardEmulator = keyboardLastEvent;
}

void EmulatorControls::UpdateKeyboardOnEventUp(const Application &app, int key) {
    if (keyboardLastEvent == nullptr) {
        keyboardLastEvent = static_cast<KeyboardEvent *>(malloc(sizeof(KeyboardEvent)));
    } else {
        keyboardLastEvent->_nextVal = static_cast<KeyboardEvent *>(malloc(sizeof(KeyboardEvent)));
        keyboardLastEvent = keyboardLastEvent->_nextVal;
    }

    keyboardLastEvent->value = key;
    keyboardLastEvent->isUp = 1;
    keyboardLastEvent->_nextVal = nullptr;

    if (keyboardEmulator == nullptr)
        keyboardEmulator = keyboardLastEvent;
}

EmulatorControls::EmulatorControls() {
    keyboardLastEvent = nullptr;
    keyboardEmulator = keyboardLastEvent;

    mouseInput = static_cast<MouseEvent *>(malloc(sizeof(MouseEvent)));
}

void EmulatorControls::UpdateKeyboardInput(const Application &app, float delta) {}

