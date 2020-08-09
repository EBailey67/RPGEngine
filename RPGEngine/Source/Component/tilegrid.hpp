#pragma once

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include <iostream>

#include "../XML/pugixml.hpp"

#include "../config.hpp"
#include "colliders.hpp"
#include "tileset.hpp"
#include "../Utility/Vector2D.h"

class TileGrid
{

public:
    using id_type = TileSet::size_type;
    TileGrid(const pugi::xml_document &xml_doc, TileSet *tileset, id_type layer)
    {
        SSECS_ASSERT(tileset);

        this->tileSet = tileset;

        auto map = xml_doc.child("map");
        auto next_id = static_cast<id_type>(std::stoi(map.attribute("nextlayerid").value()));
        SSECS_ASSERT(layer < next_id && layer > 0);
        this->layer = static_cast<Layer>(layer);

        {
            auto width = std::stoi(map.attribute("width").value());
            auto height = std::stoi(map.attribute("height").value());

            cell.resize(height);
            for (auto &row : cell)
            {
                row.resize(width);
            }
        }
        {

            auto finded = map.find_child_by_attribute("layer", "id", std::to_string(layer).c_str());

            std::string data = finded.child("data").text().get();

            for (auto &row : cell)
            {
                for (auto &col : row)
                {
	                const auto pos = data.find_first_of(",");
                    col = std::stoi(data.substr(0, pos));

                    data = data.substr(pos + 1);
                }
            }
        }
    }

    Layer layer;
    TileSet *tileSet;
    std::vector<std::vector<id_type>> cell;
    Vector2D scale;
    inline static bool hasDebugDraw = false;
};
