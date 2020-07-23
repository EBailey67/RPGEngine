#include "../core.hpp"
#include "../systems/systems.hpp"
#include "UIPanel.h"


UIPanel::UIPanel(int x, int y, int w, int h)
{
    bounds.x = x;
    bounds.y = y;
    bounds.w = w;
    bounds.h = h;
}

UIPanel::~UIPanel()
{

}

void UIPanel::OnRender()
{
    auto cameraView = registry.view<Camera>();
    auto camera = cameraView.get(*cameraView.begin());

    if (camera.Contains(GetBounds()))
    {
        Graphics::SetDrawColor(color.r, color.g, color.b, color.a);
        Graphics::DrawFillRectToLayer(Layer::UI, &bounds);
        Graphics::ResetDrawColor();

        for (auto& [name, e] : children)
        {
            if (e->IsActive())
                e->OnRender();
        }
    }
}