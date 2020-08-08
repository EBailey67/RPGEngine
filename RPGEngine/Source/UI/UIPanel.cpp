#include "../core.hpp"
#include "../Systems/systems.hpp"
#include "UIPanel.h"


UIPanel::UIPanel(const int x, const int y, const int w, const int h)
{
    bounds.x = x;
    bounds.y = y;
    bounds.w = w;
    bounds.h = h;
}

void UIPanel::OnRender()
{
	const auto cameraView = registry.view<Camera>();
	const auto& camera = cameraView.get(*cameraView.begin());

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
