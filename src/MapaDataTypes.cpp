#include "../include/MapaDataTypes.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
using json = nlohmann::json;

ObstacleData MapLoader::parseObstacle(const json &jsonObstacle)
{
    ObstacleData obstacle;
    try
    {
        obstacle.id = jsonObstacle.at("id").get<std::string>();
        obstacle.x = jsonObstacle.at("x").get<float>();
        obstacle.y = jsonObstacle.at("y").get<float>();
        obstacle.width = jsonObstacle.at("w").get<float>();
        obstacle.height = jsonObstacle.at("h").get<float>();
        obstacle.textureId = jsonObstacle.at("texture").get<std::string>();
        return obstacle;
    }
    catch (const json::exception &e)
    {
        throw std::runtime_error("Error al parsear ObstacleData en json: " + std::string(e.what()));
    }
}

EntityData MapLoader::parseEntity(const json &jsonEntity)
{
    EntityData entity;
    try
    {
        entity.id = jsonEntity.at("id").get<std::string>();
        entity.type = jsonEntity.at("type").get<std::string>();
        entity.x = jsonEntity.at("x").get<float>();
        entity.y = jsonEntity.at("y").get<float>();
        entity.textureId = jsonEntity.at("texture").get<std::string>();
        return entity;
    }
    catch (const json::exception &e)
    {
        throw std::runtime_error("Error al parsear EntityData en json: " + std::string(e.what()));
    }
}

DecorationData MapLoader::parseDecoration(const json &jsonDecoration)
{
    DecorationData decoration;
    try
    {
        decoration.id = jsonDecoration.at("id").get<std::string>();
        decoration.x = jsonDecoration.at("x").get<float>();
        decoration.y = jsonDecoration.at("y").get<float>();
        decoration.width = jsonDecoration.at("w").get<float>();
        decoration.height = jsonDecoration.at("h").get<float>();
        decoration.textureId = jsonDecoration.at("texture").get<std::string>();
        return decoration;
    }
    catch (const json::exception &e)
    {
        throw std::runtime_error("Error al parsear DecorationData en json: " + std::string(e.what()));
    }
}

MapData MapLoader::loadMap(const std::string &filename)
{
    std::ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo de mapa: " + filename);
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    fileStream.close();

    json jsonData;
    try
    {
        jsonData = json::parse(buffer.str());
    }
    catch (const json::exception &e)
    {
        throw std::runtime_error("Error al parsear JSON del mapa: " + std::string(e.what()));
    }

    MapData mapData;
    try
    {
        mapData.mapId = jsonData.at("map_id").get<std::string>();
        mapData.width = jsonData.at("width").get<float>();
        mapData.height = jsonData.at("height").get<float>();
        mapData.backgroundTextureId = jsonData.at("background_texture").get<std::string>();
        mapData.type_tree = jsonData.at("tree_texture").get<std::string>();

        for (const auto &jsonObstacle : jsonData.at("static_obstacles"))
        {
            mapData.staticObstacles.push_back(parseObstacle(jsonObstacle));
        }

        for (const auto &jsonEntity : jsonData.at("entities"))
        {
            mapData.entities.push_back(parseEntity(jsonEntity));
        }

        for (const auto &jsonDecoration : jsonData.at("decorations"))
        {
            mapData.decorations.push_back(parseDecoration(jsonDecoration));
        }
        }
    catch (const json::exception &e)
    {
        throw std::runtime_error("Error al extraer datos del mapa desde JSON: " + std::string(e.what()));
    }

    return mapData;
}