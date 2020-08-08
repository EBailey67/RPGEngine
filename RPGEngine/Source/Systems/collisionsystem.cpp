#include "collisionsystem.hpp"

#include "../core.hpp"
#include "playerinputsystem.hpp"
#include "../game_scene.hpp"

std::pair<Vector2D, Vector2D> AABBW(const SDL_FRect &lhs, const SDL_FRect &rhs)
{
    if (!((lhs.x > rhs.x + rhs.w) || (lhs.x + lhs.w < rhs.x) || (lhs.y > rhs.y + rhs.h) || (lhs.y + lhs.h < rhs.y)))
    {
        if (lhs.x < rhs.x && lhs.y < rhs.y)
        {
            return std::make_pair(Vector2D::Right(), Vector2D::Up());
        }
        if (lhs.x > rhs.x && lhs.y < rhs.y)
        {
            return std::make_pair(Vector2D::Left(), Vector2D::Up());
        }
        if (lhs.x < rhs.x && lhs.y > rhs.y)
        {
            return std::make_pair(Vector2D::Right(), Vector2D::Down());
        }
        if (lhs.x > rhs.x && lhs.y > rhs.y)
        {
            return std::make_pair(Vector2D::Left(), Vector2D::Down());
        }
        if (lhs.x < rhs.x)
        {
            return std::make_pair(Vector2D::Right(), Vector2D::Zero());
        }
        if (lhs.x > rhs.x)
        {
            return std::make_pair(Vector2D::Left(), Vector2D::Zero());
        }
        if (lhs.y < rhs.y)
        {
            return std::make_pair(Vector2D::Up(), Vector2D::Zero());
        }
        if (lhs.y > rhs.y)
        {
            return std::make_pair(Vector2D::Down(), Vector2D::Zero());
        }
        return std::make_pair(Vector2D::One(), Vector2D::One());
    }

    return std::make_pair(Vector2D::Zero(), Vector2D::Zero());
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
                if (xDir == Vector2D::Left() && vel.x < 0)
                {
                    vel.x = 0;
                }
                else if (xDir == Vector2D::Right() && vel.x > 0)
                {
                    vel.x = 0;
                }
                if (yDir == Vector2D::Up() && vel.y > 0)
                {
                    vel.y = 0;
                }
                else if (yDir == Vector2D::Down() && vel.y < 0)
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
                    Vector2D parent_pos{};

                    SDL_FRect frect{position.position.x + parent_pos.x + rect.rect.x,
                                    position.position.y + parent_pos.y + rect.rect.y, rect.rect.w, rect.rect.h};

                    auto direction = AABBW(frect, prev_frect);
                    if (direction.first != Vector2D::Zero() && direction.second != Vector2D::Zero())
                    {
                        registry.ctx<collision_signal>().publish(
                            {entt, frect, direction},
                            {entity,
                             prev_frect,
                             {Vector2D::Zero() - direction.first, Vector2D::Zero() - direction.second}});
                    }
                }
            }
        }
    }
}

void CollisionTileDetection(const float dt)
{
    auto viewRect = registry.view<RectCollider, Position, CollisionLayer, Velocity, Active>();
    auto viewGrid = registry.view<TileGridCollider, Position, TileGrid, CollisionLayer, Active>();
    for (auto rect_entt : viewRect)
    {

        auto &&[rect_pos, rect_collider, rect_vel, rect_layer] =
            viewRect.get<Position, RectCollider, Velocity, CollisionLayer>(rect_entt);

        SDL_FRect world_rect{rect_pos.position.x + rect_vel.x * dt + rect_collider.rect.x,
                             rect_pos.position.y + rect_vel.y * dt + rect_collider.rect.y, rect_collider.rect.w,
                             rect_collider.rect.h};
        for (auto grid_entt : viewGrid)
        {

            auto &&[grid_pos, tile_grid, tile_layer] = viewGrid.get<Position, TileGrid, CollisionLayer>(grid_entt);
            if (true) // rect_entt != grid_entt && CollisionLayer::Matrix[rect_layer.layer][tile_layer.layer])
            {
                SDL_FRect world_tile{grid_pos.position.x, grid_pos.position.y,
                                     tile_grid.tileSet->TileWidth() * tile_grid.scale.x,
                                     tile_grid.tileSet->TileHeight() * tile_grid.scale.y};
                int j = static_cast<int>(tile_grid.cell.size()) - 1;
                for (auto &row : tile_grid.cell)
                {
                    world_tile.y = grid_pos.position.y + j * world_tile.h;
                    int i = 0;
                    for (auto &id : row)
                    {
                        if (id)
                        {
                            world_tile.x = grid_pos.position.x + i * world_tile.w;
                            auto direction = AABBW(world_rect, world_tile);
                            if (direction.first != Vector2D::Zero() && direction.second != Vector2D::Zero())
                            {
                                //                                registry.ctx<collision_signal>().publish(
                                //                                    {rect_entt, world_rect, direction},
                                //                                    {grid_entt,
                                //                                     world_tile,
                                //                                     {Vector2D::zero() - direction.first,
                                //                                     Vector2D::zero() - direction.second}});
                                if (tile_layer.layer == LayersID::WALLS)
                                {
                                    if (direction.first == Vector2D::Left() && rect_vel.x < 0)
                                    {
                                        rect_vel.x = 0;
                                    }
                                    else if (direction.first == Vector2D::Right() && rect_vel.x > 0)
                                    {

                                        rect_vel.x = 0;
                                    }
                                    if (direction.second == Vector2D::Up() && rect_vel.y > 0)
                                    {
                                        rect_vel.y = 0;
                                    }
                                    else if (direction.second == Vector2D::Down() && rect_vel.y < 0)
                                    {
                                        rect_vel.y = 0;
                                    }
                                }
                                //                                                                if (tile_layer.layer
                                //                                                                == LayersID::FLOOR)
                                //                                                                {
                                //                                                                    auto &player =
                                //                                                                    viewRect.get<Player>(rect_entt);
                                //                                                                    player.i = i;
                                //                                                                    player.j = j;
                                //                                                                }
                            }
                        }

                        i++;
                    }
                    j--;
                }
            }
        }
    };
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
	auto* game = Instances::GetGameInstance();
    auto* gs = dynamic_cast<GameScene *>(game->Scene());
    gs->label_health->SetText("Health :" + std::to_string(hp));
}

void SetPlayerScore(const int score)
{
	auto* game = Instances::GetGameInstance();
    auto* gs = dynamic_cast<GameScene*>(game->Scene());
    gs->label_score->SetText("Score :" + std::to_string(score));
}
