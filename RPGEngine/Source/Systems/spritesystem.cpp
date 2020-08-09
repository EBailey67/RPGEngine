#include "spritesystem.hpp"

#include "../core.hpp"
#include "../UI/UIElement.h"

void SpriteRender()
{
    auto view = registry.view<Sprite, Position, Active>();
    const auto cameraView = registry.view<Camera>();
    const auto& camera = cameraView.get(*cameraView.begin());

    for (const auto &entity : view)
    {
	    auto sprite = view.get<Sprite>(entity);
        if (!sprite.isHidden)
        {
	        const auto position = view.get<Position>(entity);

            SDL_FRect rect{position.position.x, position.position.y,
            	static_cast<float>(sprite.rect.w) * sprite.scale.x,
	            static_cast<float>(sprite.rect.h) * sprite.scale.y};

            if (camera.Contains(rect))
            {
	            auto dst_rect = camera.FromWorldToScreenRect(rect);
                SDL_SetTextureAlphaMod(sprite.texture, sprite.color.a);

                Graphics::RenderToLayer(sprite.layer, sprite.texture, &sprite.rect, &dst_rect,
                                             (sprite.isFliped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));

                SDL_SetTextureAlphaMod(sprite.texture, SDL_ALPHA_OPAQUE);
            }
        }
    };
}

void PlayerCreate()
{
	const auto id = registry.create();
    auto &sprite = registry.emplace<Sprite>(id);
    auto &pos = registry.emplace<Position>(id);
    registry.emplace<Velocity>(id);
    auto &animation = registry.emplace<AnimationPool>(id);
    registry.emplace<Player>(id);
    registry.emplace<Health>(id, 3);
    registry.emplace<Active>(id);
    auto &speed = registry.emplace<MovementSpeed>(id);
    auto &rect = registry.emplace<RectCollider>(id);

    registry.emplace<NullVelocityCollision>(id);
    registry.emplace<CollisionLayer>(id, LayersID::PLAYER);
    registry.emplace<ParticleData>(id);
    registry.emplace<Dash>(id);

    const auto idle = spriteSheet.GetTypeFamily("knight_idle");
    const auto run = spriteSheet.GetTypeFamily("knight_run");

    animation.data.emplace("idle", Animation{idle, 0.07f, 0, 0});
    animation.data.emplace("run", Animation{run, 0.07f, 0, 0});
    animation.current = "idle";
    animation.isPlaying = true;

    pos.position = {550, 746};

    sprite.texture = textureCache.resource("spritesheet");

    sprite.scale = {2, 2};
    sprite.rect = spriteSheet[spriteSheet("knight_idle").first->second];
    sprite.layer = Layer::Mobs;
    sprite.isFliped = false;

    rect.rect.w = static_cast<float>(sprite.rect.w) * sprite.scale.x;
    rect.rect.h = static_cast<float>(sprite.rect.h) * sprite.scale.y;

    speed.speed = 400.f;

    const auto attack = registry.create();
    [[maybe_unused]] auto &attack_pos = registry.emplace<Position>(attack);
    auto &attack_rect = registry.emplace<RectCollider>(attack);
    auto &attack_animation = registry.emplace<AnimationPool>(attack);
    registry.emplace<Active>(attack);

    registry.emplace<CollisionLayer>(attack, LayersID::PLAYER);

    auto &attack_hierarchy = registry.emplace<Hierarchy>(attack);
    attack_hierarchy.parent = id;
    auto &parent = registry.emplace<Hierarchy>(id);
    parent.child = attack;

    auto &attack_sprite = registry.emplace<Sprite>(attack);
    attack_sprite.texture = textureCache.resource("spritesheet");

    attack_sprite.scale = {2, 2};
    attack_sprite.rect = spriteSheet[spriteSheet("attack").first->second];
    attack_sprite.layer = Layer::Mobs;
    attack_sprite.isFliped = false;
    attack_sprite.isHidden = true;

    attack_rect.rect.w = attack_sprite.scale.x * static_cast<float>(attack_sprite.rect.w);
    attack_rect.rect.h = attack_sprite.scale.y * static_cast<float>(attack_sprite.rect.h);

    const auto animation_type = spriteSheet.GetTypeFamily("attack");

    attack_animation.data.emplace("attack", Animation{animation_type, 0.05f, 0, 0});

    attack_animation.current = "attack";
    attack_animation.isPlaying = false;
}

void AnimationUpdate(const float dt)
{
    auto view = registry.view<Sprite, Position, AnimationPool, Active>();
    //const auto cameraView = registry.view<Camera>();
    //auto camera = cameraView.get(*cameraView.begin());

    for (const auto& entity : view)
    {

        auto &animationPool = view.get<AnimationPool>(entity);
        auto &animation = animationPool.data.at(animationPool.current);
        auto &sprite = view.get<Sprite>(entity);
        if (animation.current >= animation.data.size())
        {
            animation.current = 0;
        }
        animation.time += dt;
        if (animation.time > animation.rate)
        {
            sprite.rect = animation.data[animation.current];
            animation.time = 0;
            animation.current++;
        }
    }
}


void ParticleUpdate(const float dt)
{
    auto view = registry.view<ParticleData, Sprite, Active>();

    for (auto& entity: view)
    {
        auto& particle = view.get<ParticleData>(entity);
        auto& sprite = view.get<Sprite>(entity);

        if (particle.isPlaying)
        {
            particle.dt += dt;
            if (particle.dt > particle.rate)
            {
                particle.isPlaying = false;
                particle.dt = 0;
                sprite.color.a = SDL_ALPHA_OPAQUE;
            }
            else
            {
                if (sprite.color.a >= SDL_ALPHA_OPAQUE && !particle.isTranspereting)
                {
                    particle.isTranspereting = !particle.isTranspereting;
                }
                else if (sprite.color.a <= 20 && particle.isTranspereting)
                {
                    particle.isTranspereting = !particle.isTranspereting;
                }
                sprite.color.a += (particle.isTranspereting ? -particle.step : particle.step);
                sprite.color.r = 255;
            }
        }
    }
}

void UIPanelsRender()
{
    //auto view = registry.view<UIPanel, Active>();
    //auto cameraView = registry.view<Camera>();
    //auto camera = cameraView.get(*cameraView.begin());

    //for (auto& entt : view)
    //{
    //    auto& uiPanel = view.get<UIPanel>(entt);
    //    if (camera.Contains(uiPanel.rect))
    //    {
    //        Graphics::SetDrawColor(uiPanel.color.r, uiPanel.color.g, uiPanel.color.b, uiPanel.color.a);
    //        Graphics::DrawFillRectToLayer(Layer::UI, &uiPanel.rect);
    //        Graphics::ResetDrawColor();
    //    }
    //};

}

void UILabelsRender()
{
    //auto view = registry.view<UILabel, Active>();
    //auto cameraView = registry.view<Camera>();
    //auto camera = cameraView.get(*cameraView.begin());

    //for (auto& entt : view)
    //{
    //    auto& uiLabel = view.get<UILabel>(entt);

    //    SDL_Texture* labelTex = ResourceLoader::Text(fontCache.resource(GetUIFontName(uiLabel.font)), uiLabel.text, { 255, 255, 255, 255 });
    //    SDL_Rect destRect;
    //    destRect.x = uiLabel.location.x;
    //    destRect.y = uiLabel.location.y;
    //    SDL_QueryTexture(labelTex, nullptr, nullptr, &destRect.w, &destRect.h);

    //    SDL_Rect srcRect = { 0, 0, destRect.w, destRect.h };
    //    if (camera.Contains(destRect))
    //    {
    //        SDL_SetTextureAlphaMod(labelTex, 255);
    //        Graphics::RenderToLayer(Layer::UI, labelTex, &srcRect, &destRect);
    //    }
    //    SDL_DestroyTexture(labelTex);
    //};

}


void ResetTable()
{
    auto view = registry.view<entt::tag<"table"_hs>, Label>();
    auto i = ScoreTable::table_size - 1;
    for (const auto& entt : view)
    {
        auto &label = view.get<Label>(entt);
        std::string text;
        if (scoreTable.table[i].second > 0)
        {
            text = scoreTable.table[i].first + "   " + std::to_string(scoreTable.table[i].second);
        }
        else
        {
            text = " ";
        }
        textureCache.load("table" + std::to_string(i),
                          ResourceLoader::Text(fontCache.resource(GetUIFontName(UIFont::Default)), text, {255, 255, 255, 255}));
        label.AssignTexture(textureCache.resource("table" + std::to_string(i)));
        i--;
    }
}

void ResetName()
{
    auto view = registry.view<entt::tag<"name"_hs>, Label>();
    auto &label = view.get<Label>(*view.begin());

    auto view_player = registry.view<Player>();
    for (auto &entt : view_player)
    {
        auto &player = view_player.get<Player>(entt);
        textureCache.load("name",
                          ResourceLoader::Text(fontCache.resource(GetUIFontName(UIFont::Default)),
                                                    (player.name.empty() ? " " : player.name), {255, 255, 255, 255}));

        label.AssignTexture(textureCache.resource("name"));
    }
}
