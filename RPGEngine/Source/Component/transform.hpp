#pragma once

#include "../config.hpp"
#include "../Utility/Vector2D.h"
#include <array>
#include <fstream>

struct Position
{
    Vector2Df position;
    inline static bool hasDebugDraw = false;
};

struct Velocity
{
    float x;
    float y;
};

struct MovementSpeed
{
    float speed;
};


struct ScoreTable
{
    inline static constexpr auto table_size = 3;
    std::array<std::pair<std::string, int>, table_size> table{};

    void Open(std::string_view path)
    {
        std::ifstream file;
        file.open(path.data());

        if (!file.is_open())
        {
            FAST_THROW("Could't open file.");
        }
        for (auto &row : table)
        {
            std::string line;
            if (std::getline(file, line))
            {
                auto space = line.find(" ");
                row.first = line.substr(0, space);
                row.second = std::stoi(line.substr(space + 1));
            };
        }
        file.close();
    }
    void Save(std::string_view path)
    {
        std::ofstream file;
        file.open(path.data(), std::ofstream::trunc);
        for (auto &row : table)
        {
            if (row.second == 0)
            {
                break;
            }
            file << row.first;
            file << " ";
            file << std::to_string(row.second);
            file << "\n";
        }
        file.close();
    }
};

class Transform
{
public:
private:
    Vector2Df position;
    Vector2Df scale;
    // Vector2Df rotation;
};
