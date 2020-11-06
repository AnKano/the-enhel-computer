//
// Created by anshu on 11/6/2020.
//

#ifndef SAMPLEPROJECT_EMULATORCONTROLS_H
#define SAMPLEPROJECT_EMULATORCONTROLS_H

#include "EmuDeclares.h"
#include "Controls.h"

using namespace Urho3D;

class EmulatorControls : public Controls {
private:
    IntVector2 mouseHist;
public:
    EmulatorControls();

    void UpdateMouseInput(const Application &app) override;

    void UpdateKeyboardInput(const Application &app, float delta) override;

    void UpdateKeyboardOnEventDown(const Application &app, int key) override;

    void UpdateKeyboardOnEventUp(const Application &app, int key) override;
};


#endif //SAMPLEPROJECT_EMULATORCONTROLS_H
