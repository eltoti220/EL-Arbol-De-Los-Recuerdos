#ifndef MAPDATATYPES_HPP
#define MAPDATATYPES_HPP

#include <SFML/Graphics.hpp>
#include <string>

// spawn de enemigos
struct EnemySpawnData
{
    std::string enemyType;
    sf::Vector2f position;
    // float spawnTime; // tiempo despues del inicio del mapa para spawnear //*no estoy seguro de colocar eso, hay que ver si lo podemos desarrollar bien asi
    int level = 1;
};

struct PuzzleData
{
    int puzzleID;
    std::string puzzleType;
    sf::Vector2f position; //! OJO area donde se activa el puzzle o se encuentra
    bool isSolved = false;
    std::string datafile; // archivo con datos especificos del puzzle
};

// datos de checkpoints
struct CheckpointData
{
    sf::Vector2f position;
    int checkpointID;
    bool isActive = false;
};

// structura para definir la aparicion de un jefe
struct BossSpawnData
{
    std::string bossType;
    sf::Vector2f position;
    // float spawnTime; // tiempo despues del inicio del mapa para spawnear //*no estoy seguro de colocar eso, hay que ver si lo podemos desarrollar bien asi
    std::string introCinematicFile; // archivo de video para la intro del boss
};
#endif