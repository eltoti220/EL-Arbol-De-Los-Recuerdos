#ifndef MAPDATATYPES_HPP
#define MAPDATATYPES_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

namespace MapaData
{
    //*Estas constantes son solo valores predeterminados o para el mapa de inicio.
    const float MAP_WIDTH_INIT = 256.0f * 7.0f;
    const float MAP_HEIGHT_INIT = 144.0f * 5.0f;
}

struct ObstacleData
{
    std::string id;
    float x;
    float y;
    float width;
    float height;
    std::string textureId; // Para usar con TextureManager
};

struct EntityData
{
    std::string id;
    std::string type; // Ejemplo: "Enemy", "Puzzle"
    float x;
    float y;
    std::string textureId;
};
struct DecorationData
{
    std::string id;
    float x;
    float y;
    float width;
    float height;
    std::string textureId;
};
struct MapData
{
    std::string mapId;
    float width;
    float height;
    std::string backgroundTextureId;
    std::string type_tree;
    std::vector<ObstacleData> staticObstacles;
    std::vector<EntityData> entities;
    std::vector<DecorationData> decorations;
};

class MapLoader
{
public:
    static MapData loadMap(const std::string &filename);

private:
    static ObstacleData parseObstacle(const nlohmann::json &jsonObstacle);
    static EntityData parseEntity(const nlohmann::json &jsonEntity);
    static DecorationData parseDecoration(const nlohmann::json &jsonDecoration);
};

#endif