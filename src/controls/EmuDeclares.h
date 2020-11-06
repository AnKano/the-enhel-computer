//
// Created by anshu on 11/6/2020.
//

#ifndef SAMPLEPROJECT_EMUDECLARES_H
#define SAMPLEPROJECT_EMUDECLARES_H

struct KeyboardEvent {
    int value;
    int isUp;
    KeyboardEvent *_nextVal;
};

extern struct KeyboardEvent *keyboardLastEvent;
extern struct KeyboardEvent *keyboardEmulator;

struct MouseEvent {
    int mouseX = 0;
    int mouseY = 0;
    int clicked = 0;
};

extern struct MouseEvent *mouseInput;

#endif //SAMPLEPROJECT_EMUDECLARES_H
