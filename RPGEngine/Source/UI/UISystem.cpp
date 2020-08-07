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


UISystem::UISystem() = default;
UISystem::~UISystem() = default;

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

const UIElement* UISystem::AddComponent(const std::string& name, const std::shared_ptr<UIElement>& element)
{
    elements.emplace(name, element);
    return elements[name].get();
}

const UIElement* UISystem::FindComponent(const std::string& name)
{
    return nullptr;
}
