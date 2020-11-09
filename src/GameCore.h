//
// Created by anshu on 11/4/2020.
//

#ifndef SAMPLEPROJECT_GAMECORE_H
#define SAMPLEPROJECT_GAMECORE_H


#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>

#include <Urho3D/Core/Thread.h>
#include <Urho3D/Core/WorkQueue.h>

#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Audio/SoundListener.h>
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Math/Plane.h>
#include <Urho3D/Audio/BufferedSoundStream.h>
#include <Urho3D/Audio/SoundSource.h>

#include "controls/EmulatorControls.h"
#include "controls/PlayerControls.h"

#include <vector>

using namespace Urho3D;

class GameCore : public Application {
public:
    SharedPtr<Scene> scene_;
    SharedPtr<Urho3D::Window> window;

    Node *cameraNode_;

    Urho3D::Text *window_text;
    Urho3D::Terrain *terrain;
    Urho3D::Camera *camera_;

    SharedPtr<Node> skyNode;
    SharedPtr<Node> node_torch;
    SharedPtr<Node> lightNode;
    SharedPtr<Node> soundNode;

    SharedPtr<SoundSource> soundSource;
    SharedPtr<BufferedSoundStream> soundStream;

    SharedPtr<Urho3D::Texture2D> renderTexture;

    EmulatorControls emuControls;
    PlayerControls playerControls;

    std::vector<Controls*> controls = {&emuControls, &playerControls};

    int controlIterator = 0;

    unsigned char **image = static_cast<unsigned char **>(malloc(sizeof(unsigned char *)));
    int **dimx = static_cast<int **>(malloc(sizeof(int *)));
    int **dimy = static_cast<int **>(malloc(sizeof(int *)));
    int **depth = static_cast<int **>(malloc(sizeof(int *)));
    unsigned int *audio_count = static_cast<unsigned int *>(malloc(sizeof(unsigned int)));
    byte **audio_buffer = static_cast<byte **>(malloc(sizeof(byte *)));
    unsigned int *audio_mutex = static_cast<unsigned int *>(malloc(sizeof(unsigned int)));

    GameCore(Context *context) : Application(context) {}

    ~GameCore() {};

    void Setup() override;

    void Start() override;

    void Stop() override;

    void ExecuteEmulator();

    void TurnOffEmulator();

    void UpdateSound();

    void HandleUpdate(StringHash eventType, VariantMap &eventData);

    void HandleKeyDown(StringHash eventType, VariantMap &eventData);

    void HandleKeyUp(StringHash eventType, VariantMap &eventData);
};


#endif //SAMPLEPROJECT_GAMECORE_H
