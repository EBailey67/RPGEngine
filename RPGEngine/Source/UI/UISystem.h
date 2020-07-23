#pragma once

#include <string>
#include <memory>
#include <map>

#include "UIElement.h"
#include "UIPanel.h"
#include "UILabel.h"



class UISystem
{
public:
    UISystem();
    ~UISystem();

    void OnFixedUpdate();
    void OnUpdate([[maybe_unused]] const float dt);
    void OnInputUpdate();
    void OnRender();
    const UIElement* AddComponent(std::string name, std::unique_ptr<UIElement> element);

private:
    std::map<std::string, std::unique_ptr<UIElement>> elements;
};