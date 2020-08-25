#include "collisionsystem.hpp"

#include "../core.hpp"
#include "playerinputsystem.hpp"
#include "../game_scene.hpp"
#include "../sdlgui/widget.h"

std::pair<Vector2Df, Vector2Df> AABBW(const SDL_FRect &lhs, const SDL_FRect &rhs)
{
    if (!((lhs.x > rhs.x + rhs.w) || (lhs.x + lhs.w < rhs.x) || (lhs.y > rhs.y + rhs.h) || (lhs.y + lhs.h < rhs.y)))
    {
        if (lhs.x < rhs.x && lhs.y < rhs.y)
        {
            return std::make_pair(Vector2Df::Right(), Vector2Df::Up());
        }
        if (lhs.x > rhs.x && lhs.y < rhs.y)
        {
            return std::make_pair(Vector2Df::Left(), Vector2Df::Up());
        }
        if (lhs.x < rhs.x && lhs.y > rhs.y)
        {
            return std::make_pair(Vector2Df::Right(), Vector2Df::Down());
        }
        if (lhs.x > rhs.x && lhs.y > rhs.y)
        {
            return std::make_pair(Vector2Df::Left(), Vector2Df::Down());
        }
        if (lhs.x < rhs.x)
        {
            return std::make_pair(Vector2Df::Right(), Vector2Df::Zero());
        }
        if (lhs.x > rhs.x)
        {
            return std::make_pair(Vector2Df::Left(), Vector2Df::Zero());
        }
        if (lhs.y < rhs.y)
        {
            return std::make_pair(Vector2Df::Up(), Vector2Df::Zero());
        }
        if (lhs.y > rhs.y)
        {
            return std::make_pair(Vector2Df::Down(), Vector2Df::Zero());
        }
        return std::make_pair(Vector2Df::One(), Vector2Df::One());
    }

    return std::make_pair(Vector2Df::Zero(), Vector2Df::Zero());
}

void NullVelocity(const CollisionData &lhs, const CollisionData &rhs)
{
    auto worker = [](const CollisionData &col, const CollisionData &other) {
        if (registry.has<NullVelocityCollision, Velocity>(col.id) && registry.has<TileGrid>(other.id))
        {
            if ((registry.get<CollisionLayer>(other.id).layer == LayersID::WALLS))
            {

                auto &&[xDir, yDir] = col.direction;
                auto &&[vel, pos] = registry.get<Velocity, Position>(col.id);
                if (xDir == Vector2Df::Left() && vel.x < 0)
                {
                    vel.x = 0;
                }
                else if (xDir == Vector2Df::Right() && vel.x > 0)
                {
                    vel.x = 0;
                }
                if (yDir == Vector2Df::Up() && vel.y > 0)
                {
                    vel.y = 0;
                }
                else if (yDir == Vector2Df::Down() && vel.y < 0)
                {
                    vel.y = 0;
                }
            }
        }
    };
    worker(lhs, rhs);
    worker(rhs, lhs);
}

bool AABB(const SDL_FRect &lhs, const SDL_FRect &rhs)
{
    if (!((lhs.x > rhs.x + rhs.w) || (lhs.x + lhs.w < rhs.x) || (lhs.y > rhs.y + rhs.h) || (lhs.y + lhs.h < rhs.y)))
    {
        return true;
    }
    return false;
}

void CollisionDetection()
{
    auto viewRect = registry.view<RectCollider, Position, CollisionLayer, Hierarchy, Active>();
    for (const auto &entity : viewRect)
    {
        auto &&[prev_rect, prev_position, prev_layer, prev_hierarchy] =
            viewRect.get<RectCollider, Position, CollisionLayer, Hierarchy>(entity);
        SDL_FRect prev_frect{prev_position.position.x + prev_rect.rect.x,
                             prev_position.position.y + prev_rect.rect.y, prev_rect.rect.w, prev_rect.rect.h};
        for (const auto &entt : viewRect)
        {
            if (entity != entt)
            {

                auto &&[rect, position, layer, hierarchy] =
                    viewRect.get<RectCollider, Position, CollisionLayer, Hierarchy>(entt);

            	if (CollisionLayer::Matrix[layer.layer][prev_layer.layer] ||
                    CollisionLayer::Matrix[prev_layer.layer][layer.layer])
                {
                    Vector2Df parent_pos{};

                    SDL_FRect frect{position.position.x + parent_pos.x + rect.rect.x,
                                    position.position.y + parent_pos.y + rect.rect.y, rect.rect.w, rect.rect.h};

                    auto direction = AABBW(frect, prev_frect);
                    if (direction.first != Vector2Df::Zero() && direction.second != Vector2Df::Zero())
                    {
                        registry.ctx<collision_signal>().publish(
                            {entt, frect, direction},
                            {entity,
                             prev_frect,
                             {Vector2Df::Zero() - direction.first, Vector2Df::Zero() - direction.second}});
                    }
                }
            }
        }
    }
}


void CollisionMapDetection(const float dt)
{
	auto viewPlayer = registry.view<Player, Position, Velocity>();
    auto mapView = registry.view<Map, Position>();

	auto&& [pos, vel] = registry.get<Position, Velocity>(*viewPlayer.begin());
	auto&& map = registry.get<Map>(*mapView.begin());
	for (const auto& currentMap : mapView)
    {
        auto& map = mapView.get<Map>(currentMap);
		Vector2Di p(pos.position.x, pos.position.y);
		
		if (map.mapLayer == Layer::Floor)
		{
            if (vel.x < 0 && !map.cell[p.y][p.x - 1].isWalkable)
            {
            	vel.x = 0;
            }
            if (vel.x > 0 && !map.cell[p.y][p.x + 1].isWalkable)
            {
            	vel.x = 0;
            }
            if (vel.y < 0 && !map.cell[p.y - 1][p.x].isWalkable)
            {
            	vel.y = 0;
            }
            if (vel.y > 0 && !map.cell[p.y + 1][p.x].isWalkable)
            {
            	vel.y = 0;
            }
		}
    }
}

void OnHit(const CollisionData &lhs, const CollisionData &rhs)
{

    if (registry.has<Hierarchy>(lhs.id))
    {
        auto &hierarchy = registry.get<Hierarchy>(lhs.id);

        if (hierarchy.parent != entt::null)
        {

            if (registry.has<Player>(hierarchy.parent))
            {

                if (registry.has<Enemy>(rhs.id))
                {
                    auto &player = registry.get<Player>(hierarchy.parent);
                    if (player.isHit && player.isAttacking)
                    {
                        auto &particle = registry.get<ParticleData>(rhs.id);
                        if (!particle.isPlaying)
                        {

                            auto &enemy = registry.get<Health>(rhs.id);

                            particle.isPlaying = true;
                            enemy.health--;
                            if (enemy.health == 0)
                            {
                                player.score++;
                                SetPlayerScore(player.score);
                            }
                        }
                    }
                }
            }
        }
        else if (registry.has<Enemy>(lhs.id))
        {
            if (registry.has<Player>(rhs.id))
            {
                auto &player = registry.get<Health>(rhs.id);

                auto &particle = registry.get<ParticleData>(rhs.id);
                auto &enemy = registry.get<ParticleData>(lhs.id);
                if (!particle.isPlaying && !enemy.isPlaying)
                {

                    particle.isPlaying = true;
                    player.health--;
                    if (player.health == 0)
                    {
                        CloseGame();
                    }
                    else
                    {
                        SetPlayerHealth(player.health);
                    }
                }
            }
        }
    }
}

void SetPlayerHealth(const int hp)
{
	
	//auto* game = Instances::GetGameInstance();
 //   auto* gs = dynamic_cast<GameScene *>(game->Scene());
 //   gs->label_health->SetText("Health :" + std::to_string(hp));
}

void SetPlayerScore(const int score)
{
	//auto* game = Instances::GetGameInstance();
 //   auto* gs = dynamic_cast<GameScene*>(game->Scene());
 //   gs->label_score->SetText("Score :" + std::to_string(score));
}
