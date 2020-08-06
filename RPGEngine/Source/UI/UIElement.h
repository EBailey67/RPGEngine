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
    const SDL_Rect& GetBounds()
    {
        return bounds;
    }
    
    virtual bool IsActive()
    {
        return isActive;
    }

    virtual void SetActive(bool active)
    {
        isActive = active;
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

    virtual void AddChild(std::string name, std::shared_ptr<UIElement> element)
    {
        element->parent = this;
        children.emplace(name, element);
    }

protected:
    SDL_Rect bounds{0, 0, 0, 0};
    bool isActive{ false };
    UIElement* parent{ nullptr };
    std::map<std::string, std::shared_ptr<UIElement>> children;
};

