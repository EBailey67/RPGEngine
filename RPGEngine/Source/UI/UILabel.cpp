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

}

void UILabel::OnRender()
{
    auto cameraView = registry.view<Camera>();
    auto camera = cameraView.get(*cameraView.begin());

    SDL_Texture* labelTex = ResourceLoader::Text(fontCache.resource(GetUIFontName(font)), m_text, color);
    SDL_QueryTexture(labelTex, nullptr, nullptr, &bounds.w, &bounds.h);

    SDL_Rect srcRect = { 0, 0, bounds.w, bounds.h };
    if (camera.Contains(bounds))
    {
        SDL_SetTextureAlphaMod(labelTex, 255);
        Graphics::RenderToLayer(Layer::UI, labelTex, &srcRect, &bounds);
    }
    SDL_DestroyTexture(labelTex);
}

void UILabel::SetText(std::string text)
{
	m_text = text;
}