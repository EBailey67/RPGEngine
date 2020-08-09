#include "../core.hpp"
#include "UILabel.h"


UILabel::UILabel(const int x, const int y, const std::string& text)
{
	bounds.x = x;
	bounds.y = y;
	m_text = text;

    color = { 255, 255, 255, 255 };
}

UILabel::~UILabel()
{
    std::cout << "Label Deleted..." << std::endl;
}

void UILabel::OnRender()
{
    const auto camera_view = registry.view<Camera>();
    const auto& camera = camera_view.get(*camera_view.begin());
    auto dest_rect = bounds;

	if (parent != nullptr)
	{
        dest_rect = parent->GetBounds();
	}

    auto fixed_width = -1;
	if (parent != nullptr)
	{
        fixed_width = dest_rect.w - bounds.x;
	}
	
    auto* const label_tex = ResourceLoader::Text(fontCache.resource(GetUIFontName(font)), m_text, color, fixed_width);
    SDL_QueryTexture(label_tex, nullptr, nullptr, &bounds.w, &bounds.h);

    SDL_Rect src_rect = { 0, 0, bounds.w, bounds.h };

	if (parent != nullptr)
    {
        dest_rect = parent->GetBounds();
        dest_rect.x += bounds.x;
        dest_rect.y += bounds.y;
        dest_rect.w = bounds.w;
        dest_rect.h = bounds.h;
    }

    if (camera.Contains(dest_rect))
    {
        SDL_SetTextureAlphaMod(label_tex, 255);
        Graphics::RenderToLayer(Layer::UI, label_tex, &src_rect, &dest_rect);
    }

	SDL_DestroyTexture(label_tex);
}

void UILabel::SetText(const std::string& text)
{
	m_text = text;
}