#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <set>
#include <stack>
#include "Labyrinth.hpp"
using namespace sf;

struct Coordinate
{
    int x;
    int y;

    // Sobrecarga del operador de igualdad para comparar coordenadas
    bool operator==(const Coordinate& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct Box
{
    int box_x;
    int box_y;
};

//Struct que representa el estado del juego para guardar y deshacer movimientos 
struct GameState
{
    std::vector<std::vector<char>> map_state;
    std::vector<Box> box_positions;
    int player_x;
    int player_y;
};

/*
Llamado de cada elemento en el mapa:
' ' -> Espacio vacío
'P' -> Pared
'I' -> Posición inicial del jugador
'F' -> Destino / Goal
'$' -> Caja
'*' -> Caja sobre destino / Box in goal
*/

class Sokoban : public Labyrinth
{
    private:
    //Posicion de las cajas
    std::vector<Box> box_positions;
    //Posicion de los objetivos
    std::vector<Coordinate> goal_positions;
    //Pila para guardar el historial de estados del juego
    std::stack<GameState> Game_History;
    //Estado inicial del juego para reiniciar
    GameState initial_state;

    public:
    //Constructor
    Sokoban(int w, int h) : Labyrinth(w, h)
    {
    }

    //Funcion que genera el Sokoban
    void Generate_Labyrinth() override
    {
        //Hay que completarlo mas tarde

        Generate_Initial_State();
    }

    //Mueve al jugador en la direccion dada
    bool Move_Player(char direction) override
    {
        Save_State();

        int new_x = player_x;
        int new_y = player_y;
        //Cambio en x
        int dx = 0;
        //Cambio en y
        int dy = 0;

        // Determinar la nueva posición basada en la dirección
        switch (direction) 
        {
            case 'W': // Arriba / Norte (disminuye fila/x)
            dx = dr[0];
            dy = dc[0];          
            break;
            case 'S': // Abajo / Sur (aumenta fila/x)
            dx = dr[1];
            dy = dc[1]; 
            break;
            case 'A': // Izquierda / Oeste (disminuye columna/y)
            dx = dr[2];
            dy = dc[2]; 
            break;
            case 'D': // Derecha / Este (aumenta columna/y)
            dx = dr[3];
            dy = dc[3]; 
            break;
            default:
            return false; // Dirección no válida
        }  

        new_x = player_x + dx;
        new_y = player_y + dy;

        //Verificar Límites del Mapa
        if (new_x < 0 || new_x >= height || new_y < 0 || new_y >= width)
        { 
            return false; // Movimiento fuera del mapa
        }

        //Si es una pared no se puede mover
        if(map[new_x][new_y] == 'P')
        {
            return false;
        }
        //Verificar Transitabilidad (Si no es una pared)
        else if (map[new_x][new_y] == '$' || map[new_x][new_y] == '*') 
        {
            int box_new_x = new_x + dx;
            int box_new_y = new_y + dy;

            // Verificar si la nueva posición de la caja está dentro de los límites del mapa
            if (box_new_x < 0 || box_new_x >= height || box_new_y < 0 || box_new_y >= width || map[box_new_x][box_new_y] == 'P' || map[box_new_x][box_new_y] == '$' || map[box_new_x][box_new_y] == '*')
            {
                //Elimina el estado guardado ya que el movimiento no es valido
                Game_History.pop();
                return false; // Movimiento fuera del mapa
            }

            //Buscamos en las posiciones de las cajas cual es la que se va a mover
            for (auto& box : box_positions) 
            {
                //Cuando la consiga actualiza la posicion de esa caja
                if (box.box_x == new_x && box.box_y == new_y) 
                {
                    box.box_x = box_new_x;
                    box.box_y = box_new_y;
                    //Ya actualizo la caja, sale del bucle
                    break;
                }
            }

            char box_destination = map[box_new_x][box_new_y];

            // La caja se mueve, si PCN era un destino ahora es un destino con caja, si era espacio vacío ahora es un espacio con caja
            map[box_new_x][box_new_y] = (box_destination == 'F' ? '*' : '$');
            // Actualizar la posicion anterior del jugador. Su posicion inicial va pasar a ser un espacio vacio
            map[player_x][player_y] = (map[player_x][player_y] == '@' ? 'F' : ' ');

            player_x = new_x;
            player_y = new_y;
            
            return true; // Movimiento exitoso
        }
        else
        {
            // Actualizar la posicion anterior del jugador
            map[player_x][player_y] = (map[player_x][player_y] == '@' ? 'F' : ' ');
            map[new_x][new_y] = (map[new_x][new_y] == 'F' ? '@' : 'I');

            // Actualiza las coordenadas del jugador
            player_x = new_x;
            player_y = new_y;

            return true; // Movimiento exitoso
        }

        return false; // El camino está bloqueado por una pared ('P')
    }

    //Cheque si el jugador gano
    bool Check_Win() override
    {
        auto total_goals = goal_positions.size();
        int boxes_on_goals = 0;

        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                // Si una caja esta sobre un objetivo aumenta el contador de cajas en objetivos
                if(map[i][j] == '*')
                {
                    boxes_on_goals++;
                }
            }
        }
        return boxes_on_goals == static_cast<int>(total_goals);
    }

    //Guarda el estado actual del juego en la pila de estados
    void Save_State()
    {
        //Copia todos los aspectos del juego en un nuevo estado
        GameState current_state;
        current_state.map_state = map;
        current_state.box_positions = box_positions;
        current_state.player_x = player_x;
        current_state.player_y = player_y;
        
        //Añade el estado creado a la pila de estados
        Game_History.push(current_state);
    }

    //Deshace el ultimo movimiento realizado
    bool Undo_Move()
    {
        //Si esta vacio retorna falso, porque no hay movimientos que deshacer
        if(Game_History.empty())
        {
            return false; 
        }

        //Restaura las posiciones de las cajas
        GameState previous_state = Game_History.top();

        //Restaura el mapa
        map = previous_state.map_state;
        //Restaura las posiciones de las cajas
        box_positions = previous_state.box_positions;
        //Restaura la posicion del jugador
        player_x = previous_state.player_x;
        player_y = previous_state.player_y;

        //Elimina el estado de la pila
        Game_History.pop();

        return true;
    }

    void Generate_Initial_State()
    {
        initial_state.map_state = map;
        initial_state.box_positions = box_positions;
        initial_state.player_x = player_x;
        initial_state.player_y = player_y;
    }

    //Reinicia el Sokoban a su estado inicial
    void Reset_Sokoban()
    {
        //Asigna a cada aspecto del estado actual el estado inicial por defecto
        map = initial_state.map_state;
        box_positions = initial_state.box_positions;
        player_x = initial_state.player_x;
        player_y = initial_state.player_y;

        //Hasta que no este vacio va ir borrando todos los estados de juego guardados
        while (!Game_History.empty()) 
        {
            Game_History.pop();
        }
    }

    bool Is_DeadLocked()
    {
        //Lambda que verifica si una coordenada es una Pared o esta fuera de los limites
        auto Is_Blocked = [&](int x, int y)
        {
            //Si la coordenada esta fuera del mapa es considerada bloqueada
            if(x < 0 || x >= height || y < 0 || y >= width)
            {
                return true; 
            }
            return map[x][y] == 'P';
        };

        // Revisa todas las cajas
        for(const auto& box : box_positions)
        {
            int x = box.box_x;
            int y = box.box_y;

            //Verifica si la caja esta en una esquina bloqueada
            if((Is_Blocked(x - 1, y) && Is_Blocked(x, y - 1)) || (Is_Blocked(x - 1, y) && Is_Blocked(x, y + 1)) || (Is_Blocked(x + 1, y) && Is_Blocked(x, y - 1)) || (Is_Blocked(x + 1, y) && Is_Blocked(x, y + 1)))
            {
                // Y si la caja esta en un objetivo
                if(map[x][y] == '$')
                {
                    return true;
                }
                
            }
        }
        return false;
    }

    // Sobreescribe el renderizado para incluir cajas
    void Render_Game(sf::RenderWindow& window, const Labyrinth& game, sf::Texture& texture) override
    {
        sf::Sprite sprite(texture);
        
        // Rectángulos de Textura para Sokoban:
        // Se asume que tu piso.png tiene 6 sprites de 32x32 seguidos en la fila 0:
        // 0: Piso, 32: Pared, 64: Meta, 96: Jugador, 128: Caja, 160: Caja en Meta
        sf::IntRect floor_rect({0, 0}, {TILE_SIZE, TILE_SIZE});         
        sf::IntRect wall_rect({32, 0}, {TILE_SIZE, TILE_SIZE});         
        sf::IntRect goal_rect({64, 0}, {TILE_SIZE, TILE_SIZE});         
        sf::IntRect player_rect({96, 0}, {TILE_SIZE, TILE_SIZE});       
        sf::IntRect box_rect({128, 0}, {TILE_SIZE, TILE_SIZE});      
        sf::IntRect box_on_goal_rect({160, 0}, {TILE_SIZE, TILE_SIZE}); 

        for (int r = 0; r < game.getHeight(); ++r)
        {
            for (int c = 0; c < game.getWidth(); ++c)
            {
                char cell = game.getCell(r, c); 
                
                float pos_x = static_cast<float>(c * TILE_SIZE);
                float pos_y = static_cast<float>(r * TILE_SIZE);
                
                sprite.setPosition({pos_x, pos_y});

                // 1. Dibuja el fondo (Piso, Meta o Pared)
                if (cell == 'P')
                {
                    sprite.setTextureRect(wall_rect);
                    window.draw(sprite);
                    continue; // Pasa al siguiente tile si es pared
                }

                // Dibujar el piso por debajo de todo
                sprite.setTextureRect(floor_rect);
                window.draw(sprite); 

                // Si hay una meta, dibújala sobre el piso
                if (cell == 'F' || cell == 'X' || cell == '@') 
                {
                    sprite.setTextureRect(goal_rect);
                    window.draw(sprite);
                }
                
                // 2. Dibuja el elemento de encima (Caja o Jugador)
                if (cell == 'B')
                {
                    sprite.setTextureRect(box_rect);
                    window.draw(sprite);
                }
                else if (cell == 'X')
                {
                    sprite.setTextureRect(box_on_goal_rect);
                    window.draw(sprite);
                }
                else if (cell == 'I' || cell == '@')
                {
                    sprite.setTextureRect(player_rect);
                    window.draw(sprite);
                }
            }
        }
    }
};