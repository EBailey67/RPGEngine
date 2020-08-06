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
    const UIElement* AddComponent(std::string name, std::shared_ptr<UIElement> element);
    const UIElement* FindComponent(std::string name);

private:
    std::map<std::string, std::shared_ptr<UIElement>> elements;
};