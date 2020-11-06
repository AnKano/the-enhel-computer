//
// Created by anshu on 11/6/2020.
//

#ifndef SAMPLEPROJECT_CONTROLS_H
#define SAMPLEPROJECT_CONTROLS_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/Application.h>

using namespace Urho3D;

class Controls {
public:
    virtual void UpdateMouseInput(const Application &app) = 0;

    virtual void UpdateKeyboardInput(const Application &app, float delta) = 0;

    virtual void UpdateKeyboardOnEventDown(const Application &app, int key) = 0;

    virtual void UpdateKeyboardOnEventUp(const Application &app, int key) = 0;
};


#endif //SAMPLEPROJECT_CONTROLS_H
