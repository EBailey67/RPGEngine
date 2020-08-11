#pragma once

class BasicScene
{
public:
    virtual ~BasicScene() = default;

    virtual void FixedUpdate()
    {
    }

    virtual void Update([[maybe_unused]] const float dt)
    {
    }

    virtual void InputUpdate()
    {
    }

    virtual void Render()
    {
    }

    virtual void RenderUI()
    {
    }

	virtual void ResizeEvent()
    {
	    
    }
};
