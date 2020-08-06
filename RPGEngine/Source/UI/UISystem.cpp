#include "UISystem.h"

const char* GetUIFontName(UIFont uiFont)
{
    switch (uiFont)
    {
    case UIFont::Default:
        return "DefaultFont";
    case UIFont::Debug:
        return "DebugFont";
    default:
        return "DefaultFont";
    }
}


UISystem::UISystem()
{
}

UISystem::~UISystem()
{
}

void UISystem::OnFixedUpdate()
{
}

void UISystem::OnUpdate([[maybe_unused]] const float dt)
{
}

void UISystem::OnInputUpdate()
{
}

void UISystem::OnRender()
{
    for (auto& [name, e] : elements)
    {
        if (e->IsActive())
            e->OnRender();
    }
}

const UIElement* UISystem::AddComponent(std::string name, std::shared_ptr<UIElement> element)
{
    elements.emplace(name, element);
    return elements[name].get();
}

const UIElement* UISystem::FindComponent(std::string name)
{
    return nullptr;
}
