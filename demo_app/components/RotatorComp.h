#pragma once
#include <Transform.h>
#include <Component.h>

// #if defined(ENABLE_IMGUI)
// #include <imgui.h>
// #endif

class Rotator : public Component {
public:
    enum class Axis : int { X = 0, Y = 1, Z = 2};

    bool enabled = true;
    Axis axis;
    float speed;

    explicit Rotator(Axis axis = Axis::Y, float speed = 45.0f)
        : axis(axis), speed(speed) {}

    void render(GameObject& owner) override {}

    void update(GameObject& owner, float deltaTime) override {
        if (!enabled) return;

        if (Transform* tf = owner.getComponent<Transform>()) {
            switch (axis) {
                case Axis::X: tf->rotation.x += speed * deltaTime; break;
                case Axis::Y: tf->rotation.y += speed * deltaTime; break;
                case Axis::Z: tf->rotation.z += speed * deltaTime; break;
            }
        }
    }

// #if defined(ENABLE_IMGUI)
//     void onInspectorGUI() override {
//         ImGui::SeparatorText(inspectorName().data());
//         // ImGui::Checkbox("Enabled", &enabled);

//         // const char* items[] = { "X", "Y", "Z" };
//         // int curr = static_cast<int>(axis);
//         // if (ImGui::Combo("Axis", &curr, items, IM_ARRAYSIZE(items))) {
//         //     axis = static_cast<Axis>(curr);
//         // }

//         // ImGui::SliderFloat("Speed", &speed, -360.0f, 360.0f, "%.3f");
//     }
// #endif
};
