#include "../core.hpp"
#include "UILabel.h"


UILabel::UILabel(int x, int y, std::string text)
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
    auto cameraView = registry.view<Camera>();
    auto camera = cameraView.get(*cameraView.begin());

    SDL_Texture* labelTex = ResourceLoader::Text(fontCache.resource(GetUIFontName(font)), m_text, color);
    SDL_QueryTexture(labelTex, nullptr, nullptr, &bounds.w, &bounds.h);

    SDL_Rect srcRect = { 0, 0, bounds.w, bounds.h };
    SDL_Rect destRect = bounds;
    if (parent != nullptr)
    {
        destRect = parent->GetBounds();
        destRect.x += bounds.x;
        destRect.y += bounds.y;
        destRect.w = bounds.w;
        destRect.h = bounds.h;
    }

    if (camera.Contains(destRect))
    {
        SDL_SetTextureAlphaMod(labelTex, 255);
        Graphics::RenderToLayer(Layer::UI, labelTex, &srcRect, &destRect);
    }
    SDL_DestroyTexture(labelTex);
}

void UILabel::SetText(std::string text)
{
	m_text = text;
}