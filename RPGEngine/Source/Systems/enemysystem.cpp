#include <queue>

#include "enemysystem.hpp"
#include "../core.hpp"

void EnemyCreate(Vector2D spawn)
{
    auto enemy = registry.create();
    registry.emplace<Enemy>(enemy);

    auto &pos = registry.emplace<Position>(enemy);
    pos.position = spawn;
    
    auto &rect = registry.emplace<RectCollider>(enemy);
    auto &sprite = registry.emplace<Sprite>(enemy);
    auto &speed = registry.emplace<MovementSpeed>(enemy);
    
    [[maybe_unused]] auto &vel = registry.emplace<Velocity>(enemy);
    registry.emplace<Health>(enemy, 2);
    registry.emplace<ParticleData>(enemy);
    registry.emplace<Active>(enemy);
    speed.speed = 150;
    
    registry.emplace<CollisionLayer>(enemy, LayersID::ENEMY);

    sprite.texture = textureCache.resource("spritesheet");

    sprite.scale = {2, 2};
    sprite.rect = spriteSheet[spriteSheet("zombie_idle").first->second];
    sprite.layer = Layer::Mobs;
    sprite.isFliped = false;

    rect.rect.w = sprite.rect.w * sprite.scale.x;
    rect.rect.h = sprite.rect.h * sprite.scale.y;

    auto &animation = registry.emplace<AnimationPool>(enemy);

    auto idle = spriteSheet.GetTypeFamily("zombie_idle");
    auto run = spriteSheet.GetTypeFamily("zombie_run");

    animation.data.emplace("idle", Animation{idle, 0.07f, 0, 0});
    animation.data.emplace("run", Animation{run, 0.07f, 0, 0});

    animation.current = "idle";

    auto view = registry.create();
    auto multiplier = registry.emplace<View>(view);
    registry.emplace<Active>(view);
    registry.emplace<CollisionLayer>(view, LayersID::ENEMY);
    multiplier.multiplier = 12;
    registry.emplace<Position>(view);
    auto &view_rect = registry.emplace<RectCollider>(view);

    view_rect.rect.w = sprite.rect.w * sprite.scale.x * multiplier.multiplier;
    view_rect.rect.h = sprite.rect.h * sprite.scale.y * multiplier.multiplier;
    view_rect.rect.x = -view_rect.rect.w / 2 + sprite.rect.w * sprite.scale.x;
    view_rect.rect.y = -view_rect.rect.h / 2 + sprite.rect.h * sprite.scale.y;

    auto &parent = registry.emplace<Hierarchy>(enemy);
    parent.child = view;
    auto &child = registry.emplace<Hierarchy>(view);
    child.parent = enemy;
}

void UpdateView()
{
    auto view = registry.view<Hierarchy, Position, RectCollider, View>();
    view.each([](auto &hierarchy, auto &position, auto &rect, auto &view) 
    {
        auto parent_pos = registry.get<Position>(hierarchy.parent);
        position.position.Set(parent_pos.position.x, parent_pos.position.y);
    });
}

void EnemyCharging(const CollisionData &lhs, const CollisionData &rhs)
{
    if (registry.has<View>(lhs.id) && registry.has<Player>(rhs.id))
    {

        auto &&[player_pos, player] = registry.get<Position, Player>(rhs.id);
        auto parent = registry.get<Hierarchy>(lhs.id).parent;
        auto &&[enemy_pos, enemy_vel, enemy_speed, enemy] =
            registry.get<Position, Velocity, MovementSpeed, Enemy>(parent);

        enemy_vel.x = (player_pos.position.normalized()).x * enemy_speed.speed;
        enemy_vel.y = (player_pos.position.normalized()).y * enemy_speed.speed;
        if (enemy_pos.position.x > player_pos.position.x)
        {
            enemy_vel.x *= -1;
        }
        else
        {
            enemy_vel.x *= 1;
        }

        if (enemy_pos.position.y > player_pos.position.y)
        {
            enemy_vel.y *= -1;
        }
        else
        {
            enemy_vel.y *= 1;
        }
        enemy.isCharging = true;
        enemy.dt = 0;
    }
}

void EnemyWalking(const float dt)
{
    auto view = registry.view<Enemy, Velocity, MovementSpeed, Active>();
    for (auto&entity : view)
    {
        auto& enemy = view.get<Enemy>(entity);
        auto& vel = view.get<Velocity>(entity);
        const auto& speed = view.get<MovementSpeed>(entity);
        if (enemy.dt > enemy.time)
        {
            if (vel.x || vel.y)
            {
                vel.x = 0;
                vel.y = 0;
            }
            else
            {
                auto result = std::rand() % 2;
                if (result)
                {
                    result = std::rand() % 2;
                    if (result)
                    {
                        vel.x = 0.5f * speed.speed;
                    }
                    else
                    {
                        vel.x = -0.5f * speed.speed;
                    }
                }
                else
                {
                    result = std::rand() % 2;
                    if (result)
                    {
                        vel.y = 0.5f * speed.speed;
                    }
                    else
                    {
                        vel.y = -0.5f * speed.speed;
                    }
                }
            }
            vel.y = 0;

            enemy.dt = 0;
        }
        enemy.dt += dt;
    }
}

void HealthUpdate()
{
    auto view = registry.view<Enemy, Health, Active, Position>();

    for (auto &entity : view)
    {
        auto &health = view.get<Health>(entity);

        if (health.health <= 0)
        {
            auto &&[position, enemy] = view.get<Position, Enemy>(entity);
            position.position = Enemy::spawns[Enemy::currentSpawn];
            Enemy::currentSpawn++;
            health.health = 2;

            if (Enemy::currentSpawn >= Enemy::MAX_SPAWNS)
            {
                Enemy::currentSpawn = 0;
            }
        }
    }
}
