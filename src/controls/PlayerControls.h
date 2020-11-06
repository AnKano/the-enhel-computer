//
// Created by anshu on 11/6/2020.
//

#ifndef SAMPLEPROJECT_PLAYERCONTROLS_H
#define SAMPLEPROJECT_PLAYERCONTROLS_H

#include "Controls.h"

using namespace Urho3D;

class PlayerControls : public Controls {
public:
    PlayerControls();

    void UpdateKeyboardInput(const Application &app, float delta) override;

    void UpdateKeyboardOnEventDown(const Application &app, int key) override;

    void UpdateKeyboardOnEventUp(const Application &app, int key) override;

    void UpdateMouseInput(const Application &app) override;
};


#endif //SAMPLEPROJECT_PLAYERCONTROLS_H
