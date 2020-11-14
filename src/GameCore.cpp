#include "GameCore.h"

#include <cmath>

extern "C" {
#include "emulator/protos_urho_driver.h"
}

struct SoundEvent {
    byte *buffer;
    unsigned int size;
    SoundEvent *_nextVal;
};

SoundEvent *soundLastSample = nullptr;
SoundEvent *soundEmulator = soundLastSample;
int turnOffState = 0;

template<typename T>
void set_model(T *model, Urho3D::ResourceCache *cache, const std::string &model_name) {
    std::string filename_model = model_name;
    model->SetModel(cache->GetResource<Urho3D::Model>(Urho3D::String(filename_model.append(".mdl").c_str())));
    std::string filename_txt = model_name;
    filename_txt.append(".txt");
    std::ifstream file(filename_txt.c_str());
    std::string line;
    if (file.is_open())
        for (int i = 0; getline(file, line); i++)
            model->SetMaterial(i, cache->GetResource<Urho3D::Material>(Urho3D::String(line.c_str())));
}

void GameCore::Start() {
    Application::Start();

    auto *cache = GetSubsystem<ResourceCache>();
    GetSubsystem<UI>()->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

    scene_ = new Scene(context_);
    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<DebugRenderer>();

    cameraNode_ = scene_->CreateChild("Camera");
    camera_ = cameraNode_->CreateComponent<Camera>();
    camera_->SetFarClip(600);
    camera_->SetNearClip(0.1);
    camera_->SetFov(75);

    auto *listener = cameraNode_->CreateComponent<SoundListener>();
    GetSubsystem<Audio>()->SetListener(listener);
    GetSubsystem<Audio>()->SetMasterGain(SOUND_MASTER, 1.0);

    auto *renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
    renderer->SetShadowMapSize(1024);

    RenderPath *effectRenderPath = viewport->GetRenderPath();
    effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/AutoExposure.xml"));
    effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/FXAA2.xml"));

    Node *zoneNode = scene_->CreateChild("Zone");
    Zone *zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-50000.0f, 50000.0f));
    zone->SetFogStart(500.0f);
    zone->SetFogEnd(600.0f);
    zone->SetFogColor(Color(1, 1, 1));
    zone->SetAmbientColor(Color(0.1, 0.1, 0.1));

    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(GameCore, HandleKeyDown));
    SubscribeToEvent(E_KEYUP, URHO3D_HANDLER(GameCore, HandleKeyUp));
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(GameCore, HandleUpdate));

    cameraNode_->SetPosition(Vector3(0.0f, 5.0f, 0.0f));
    cameraNode_->SetDirection(Vector3::FORWARD);

    // create a transparent window with some text to display things like help and FPS
    {
        window = new Window(context_);
        GetSubsystem<UI>()->GetRoot()->AddChild(window);
        window->SetStyle("Window");
        window->SetSize(600, 90);
        window->SetColor(Color(.0, .15, .3, .5));
        window->SetAlignment(HA_LEFT, VA_TOP);

        window_text = new Text(context_);
        window_text->SetFont(cache->GetResource<Font>("Fonts/BlueHighway.ttf"), 14);
        window_text->SetColor(Color(.8, .85, .9));
        window_text->SetAlignment(HA_LEFT, VA_TOP);
        window->AddChild(window_text);
    }

    // sun
    {
        lightNode = scene_->CreateChild("Light");
        auto *light = lightNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetCastShadows(true);
        light->SetShadowBias(BiasParameters(0.00025f, 0.7f));
        light->SetShadowCascade(CascadeParameters(4.0f, 16.0f, 64.0f, 128.0f, 0.8f));
        light->SetColor(Color(1.4, 0.9, 0.8, 1));
        lightNode->SetDirection(Vector3::FORWARD);
        lightNode->Yaw(-150);   // horizontal
        lightNode->Pitch(30);   // vertical
        lightNode->Translate(Vector3(0, 0, -20000));

        auto *billboardObject = lightNode->CreateComponent<BillboardSet>();
        billboardObject->SetNumBillboards(1);
        billboardObject->SetMaterial(cache->GetResource<Material>("Materials/sun.xml"));
        billboardObject->SetSorted(true);
        Billboard *bb = billboardObject->GetBillboard(0);
        bb->size_ = Vector2(10000, 10000);
        bb->rotation_ = Random() * 360.0f;
        bb->enabled_ = true;
        billboardObject->Commit();
    }

    Node *screenNode = scene_->CreateChild("Screen");
    screenNode->SetPosition(Vector3(0.0f, 5.0f, 15.0f));
    screenNode->SetRotation(Quaternion(-90.0f, 0.0f, 0.0f));
    screenNode->SetScale(Vector3(20.0f, 0.0f, 15.0f));
    auto *screenObject = screenNode->CreateComponent<StaticModel>();
    screenObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));

    renderTexture = SharedPtr<Texture2D>(new Texture2D(context_));
    renderTexture->SetNumLevels(1);

    SharedPtr<Material> renderMaterial(new Material(context_));
    renderMaterial->SetTechnique(0, cache->GetResource<Technique>("Techniques/DiffUnlitBGRA.xml"));
    renderMaterial->SetTexture(TU_DIFFUSE, renderTexture);

    renderMaterial->SetDepthBias(BiasParameters(-0.001f, 0.0f));
    screenObject->SetMaterial(renderMaterial);

    soundNode = scene_->CreateChild("SoundNode");
    soundStream = new BufferedSoundStream();
    soundStream->SetFormat(48000, true, true);
    soundSource = soundNode->CreateComponent<SoundSource>();
    soundSource->Play(soundStream);
}

void GameCore::ExecuteEmulator() {
    turnOffState = 0;
    this->fu = std::async(std::launch::async, [=]() {
        return run(0, (char **) nullptr, image, dimx, dimy, depth, audio_count, audio_buffer, audio_mutex);
    });
}

void GameCore::TurnOffEmulator() {
    turnOffState = 1;
}

void GameCore::Setup() {
    Application::Setup();

    *dimx = nullptr;
    *dimy = nullptr;
    *depth = nullptr;

    *audio_buffer = nullptr;

    engineParameters_["FullScreen"] = false;
    engineParameters_["WindowWidth"] = 2560;
    engineParameters_["WindowHeight"] = 1440;
    engineParameters_["WindowResizable"] = true;
    engineParameters_["Multisample"] = 16;
}

void GameCore::Stop() {
    Application::Stop();
}

void GameCore::HandleUpdate(StringHash eventType, VariantMap &eventData) {
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

    std::string str = "WASD, mouse and shift to move. T to toggle fill mode,\nZ to change mouse pointer controls to camera controls\nG to toggle GUI, Tab to toggle mouse mode, Space to execute emulator.\n";
    {
        std::ostringstream ss;
        ss << 1 / timeStep;
        std::string s(ss.str());
        str.append(s.substr(0, 6));
    }
    str.append(" FPS ");
    String s(str.c_str(), str.size());
    window_text->SetText(s);

    controls[controlIterator]->UpdateKeyboardInput(*this, timeStep);
    controls[controlIterator]->UpdateMouseInput(*this);

    UpdateSound();

    if (*dimx != nullptr) {
        if (**dimx > 0) {
            int width = **dimx;
            int height = **dimy;

            renderTexture->SetSize(width, height, Graphics::GetRGBAFormat(), TEXTURE_DYNAMIC);
            renderTexture->SetFilterMode(FILTER_ANISOTROPIC);
            renderTexture->SetData(0, 0, 0, width, height, *image);
        }
    }
}

void GameCore::UpdateSound() {
    if (soundEmulator == nullptr)
        soundEmulator = soundLastSample;

    if (soundEmulator != nullptr) {
        SoundEvent *_prev;

        while (soundEmulator->_nextVal != nullptr) {
            soundStream->AddData(soundEmulator->buffer, soundEmulator->size);

            _prev = soundEmulator;
            soundEmulator = soundEmulator->_nextVal;

            // free useless buffer memory in C-style
            free(_prev->buffer);
            // free struct
            free(_prev);
        }
    }
}

void GameCore::HandleKeyDown(StringHash eventType, VariantMap &eventData) {
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();

    controls[controlIterator]->UpdateKeyboardOnEventDown(*this, key);

    if (key == KEY_TAB) {
        GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
        GetSubsystem<Input>()->SetMouseGrabbed(!GetSubsystem<Input>()->IsMouseGrabbed());
    } else if (key == KEY_G) {
        window->SetVisible(!window->IsVisible());
        soundStream->Clear();
    } else if (key == KEY_T)
        camera_->SetFillMode(camera_->GetFillMode() == FILL_WIREFRAME ? FILL_SOLID : FILL_WIREFRAME);
    else if (key == KEY_Z) {
        controlIterator++;
        controlIterator %= controls.size();
    } else if (key == KEY_SPACE) {
        this->ExecuteEmulator();
    }
}

void GameCore::HandleKeyUp(StringHash eventType, VariantMap &eventData) {
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();

    controls[controlIterator]->UpdateKeyboardOnEventUp(*this, key);
}