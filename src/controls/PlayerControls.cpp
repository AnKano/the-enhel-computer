//
// Created by anshu on 11/6/2020.
//

#include "PlayerControls.h"

#include "../GameCore.h"

void PlayerControls::UpdateKeyboardInput(const Application &app, float delta) {
    // Movement speed as world units per second
    float MOVE_SPEED = 10.0f;

    auto* input = app.GetSubsystem<Input>();
    const auto &b = dynamic_cast<const GameCore &>(app);
    auto *camera = b.cameraNode_;

    if (input->GetQualifierDown(1))  // 1 is shift, 2 is ctrl, 4 is alt
        MOVE_SPEED *= 10;
    if (input->GetKeyDown('W'))
        camera->Translate(Vector3(0, 0, 1) * MOVE_SPEED * delta);
    if (input->GetKeyDown('S'))
        camera->Translate(Vector3(0, 0, -1) * MOVE_SPEED * delta);
    if (input->GetKeyDown('A'))
        camera->Translate(Vector3(-1, 0, 0) * MOVE_SPEED * delta);
    if (input->GetKeyDown('D'))
        camera->Translate(Vector3(1, 0, 0) * MOVE_SPEED * delta);
}

void PlayerControls::UpdateMouseInput(const Application& app) {
    // Mouse sensitivity as degrees per pixel
    const float MOUSE_SENSITIVITY = 0.1f;

    auto* input = app.GetSubsystem<Input>();
    const auto &b = dynamic_cast<const GameCore &>(app);
    auto *camera = b.cameraNode_;

    if (!input->IsMouseVisible()) {
        IntVector2 mouseMove = input->GetMouseMove();
        if (mouseMove.x_ > -2000000000 && mouseMove.y_ > -2000000000) {
            static float yaw_ = 0;
            static float pitch_ = 0;
            yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
            pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
            pitch_ = Clamp(pitch_, -90.0f, 90.0f);
            // Reset rotation and set yaw and pitch again
            camera->SetDirection(Vector3::FORWARD);
            camera->Yaw(yaw_);
            camera->Pitch(pitch_);
        }
    }
}

void PlayerControls::UpdateKeyboardOnEventDown(const Application &app, int key) {

}

void PlayerControls::UpdateKeyboardOnEventUp(const Application &app, int key) {

}

PlayerControls::PlayerControls() = default;
