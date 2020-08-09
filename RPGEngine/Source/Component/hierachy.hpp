#pragma once

#include "entt/entity/entity.hpp"

struct Hierarchy
{
    entt::entity child{entt::null};
    entt::entity parent{entt::null};
};

struct Parent
{
    entt::entity parent;
};
