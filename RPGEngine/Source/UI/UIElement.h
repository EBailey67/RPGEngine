#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <string>

#include <SDL.h>


enum class UIFont
{
    Default,
    Debug
};

extern const char* GetUIFontName(UIFont uiFont);


class UIElement
{
public:
	[[nodiscard]] const SDL_Rect& GetBounds() const
    {
        return bounds;
    }
    
    virtual bool IsActive()
    {
        return is_active;
    }

    virtual void SetActive(const bool active)
    {
        is_active = active;
    }

    virtual ~UIElement() { std::cout << "Element deleted" << std::endl; };

    virtual void OnFixedUpdate()
    {
    }

    virtual void OnUpdate([[maybe_unused]] const float dt)
    {
    }

    virtual void OnInputUpdate()
    {
    }

    virtual void OnRender()
    {
    }

    virtual void SetParent(UIElement& element)
    {
        parent = &element;
    }

    virtual void AddChild(std::string_view name, const std::shared_ptr<UIElement>& element)
    {
        element->parent = this;
        children.emplace(name, element);
    }

protected:
    SDL_Rect bounds{0, 0, 0, 0};
    bool is_active{ false };
    UIElement* parent{ nullptr };
    std::map<std::string, std::shared_ptr<UIElement>> children;
};

