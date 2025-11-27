#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <utility>
#include <set>

/*
Llamado de cada elemento en el mapa:
' ' -> Espacio vacío
'P' -> Pared
'I' -> Posición inicial del jugador
'F' -> Destino / Goal
'@' ->Jugador sobre Destino
*/

const std::string TEXTURE_PATH = "piso.png";

class Labyrinth 
{
    protected:
    //Struct que representa el caminar del personaje
    struct point
    {
        //Coordenadas
        int x;
        int y;
        //Puntero al nodo padre para armar el camino recorrido
        point* parent;
    };

    //Ancho del laberinto
    int width;
    //Alto del laberinto
    int height;
    //Matriz que representa el laberinto
    std::vector<std::vector<char>> map;
    int dr[4] = {-1, 1, 0, 0}; // Direcciones de movimiento en x
    int dc[4] = {0, 0, -1, 1}; // Direcciones de movimiento en y
    std::vector<point*> all_nodes;
    //Movimiento del jugador
    int player_x;
    int player_y;

    // Funcion auxiliar para liberar memoria dinamica
    void Clear_Nodes()
    {
        for (point* node : all_nodes) 
        {
            delete node;
        }
        all_nodes.clear();
    }

    public:

    static const int TILE_SIZE = 32; 

    //Constructor 
    Labyrinth(int w, int h) : width(w), height(h)
    {
        map.resize(height, std::vector<char>(width, 'P'));
    }

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    char getCell(int x, int y) const
    {
        return map[x][y];
    }

    //Destructor
    virtual ~Labyrinth()
    {
        Clear_Nodes();
    }

    //Funcion que genera el laberinto
    virtual void Generate_Labyrinth()
    {
        std::vector<std::string> maze_data = 
        {
            "PPPPPPPPPPPPPPPPPPPP",
            "P I                P", 
            "P P PPPPPPPPPPPPPPPP",
            "P P P              P",
            "P P P P PPPPPP P P P",
            "P P P P P      P P P",
            "P P P P P PPPP P P P",
            "P P P P P P    P P P",
            "P P P P P P PPPP P P",
            "P P P P P P P    P P",
            "P P P P P P P PPPP P",
            "P P P P P P P P    P",
            "P P P P P P P P P PP",
            "P P P P P P P P P  P",
            "P P P P P P P P P PP",
            "P P P P P P P P P  P",
            "P P P P P P P P P PP",
            "P P P P P P P P P  P",
            "P P P P P P P P P  P",
            "PPPPPPPPPPPPPPPPPF P"
        };

        height = maze_data.size();
        width = maze_data[0].size();
        map.resize(height, std::vector<char>(width));

        //Bucle que pasa todos los strings a la matriz del juego
        for(int i = 0; i < height; ++i)
        {
            for(int j = 0; j < width; ++j)
            {
                map[i][j] = maze_data[i][j];

                if(map[i][j] == 'I')
                {
                    player_x = i;
                    player_y = j;
                }
            }
        }
    }

    //Va buscar el camino mas corto
    bool Solve_labyrinth()
    {
        std::queue<point*> q;
        std::set<std::pair<int, int>> visited;
        Clear_Nodes();

        // Encontrar la posición inicial 'I'
        point* start = new point{player_x, player_y, nullptr};
        all_nodes.push_back(start);

         q.push(start);
        visited.insert({start->x, start->y});

        while (!q.empty()) 
        {
            point* current = q.front();
            q.pop();

            // Verificar si hemos llegado al punto final 'F'
            if (map[current->x][current->y] == 'F') 
            {
                // Reconstruir el camino
                point* path_node = current;
                while (path_node) 
                {
                    if (map[path_node->x][path_node->y] != 'I' && map[path_node->x][path_node->y] != 'F') 
                    {
                        map[path_node->x][path_node->y] = '.';
                    }
                    path_node = path_node->parent;
                }
                Clear_Nodes();
                return true; 
            }

            // Explorar las cuatro direcciones posibles
            for (int i = 0; i < 4; ++i) 
            {
                int new_x = current->x + dr[i];
                int new_y = current->y + dc[i];

                // Verificar límites y si es un camino válido
                if (new_x >= 0 && new_x < height && new_y >= 0 && new_y < width && visited.find({new_x,new_y}) == visited.end()) 
                {
                    if(map[new_x][new_y] != 'P')
                    {
                        point* next_point = new point{new_x, new_y, current};
                        q.push(next_point);
                        visited.insert({new_x, new_y});
                        all_nodes.push_back(next_point);
                    }
                }
            }
        }

        return false; 
    }

    virtual bool Move_Player(char direction)
    {
        int dx;
        int dy;

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

        int new_x = player_x + dx;
        int new_y = player_y + dy;

        //Verificar Límites del Mapa
        if (new_x < 0 || new_x >= height || new_y < 0 || new_y >= width)
        { 
            return false; // Movimiento fuera del mapa
        }

        //Verificar Transitabilidad 
        if (map[new_x][new_y] != 'P') 
        {
            map[player_x][player_y] = (map[player_x][player_y] == '@' ? 'F' : ' ');

            // Actualiza las coordenadas del jugador
            player_x = new_x;
            player_y = new_y;

            map[player_x][player_y] = (map[player_x][player_y] == 'F' ? '@' : 'I');
            
            return true; // Movimiento exitoso
        }

        return false; // El camino está bloqueado por una pared ('P')
    }

    virtual bool Check_Win()
    {
        char current_cell = map[player_x][player_y];

        // Ganaste, estas sobre el destino 'F'
        if(current_cell == '@')
        {
            return true;
        }

        return false;
    }

    void Render_Game(sf::RenderWindow& window, const Labyrinth& game, sf::Texture& texture)
    {
        sf::Sprite sprite(texture);
    
        // Rectángulos de Textura (¡AJUSTA ESTOS VALORES A TU piso.png!)
        sf::IntRect floor_rect({0, 0}, {TILE_SIZE, TILE_SIZE});        
        sf::IntRect wall_rect({32, 0}, {TILE_SIZE, TILE_SIZE});        
        sf::IntRect goal_rect({64, 0}, {TILE_SIZE, TILE_SIZE});        
        sf::IntRect player_rect({96, 0}, {TILE_SIZE, TILE_SIZE});       

        // Usamos los métodos get para acceder a las dimensiones
        for (int r = 0; r < game.getHeight(); ++r)
        {
            for (int c = 0; c < game.getWidth(); ++c)
            {
                // Usamos getCell para acceder al contenido del mapa
                char cell = game.getCell(r, c); 
            
                float pos_x = static_cast<float>(c * TILE_SIZE);
                float pos_y = static_cast<float>(r * TILE_SIZE);
            
                sprite.setPosition({pos_x, pos_y});

                // Lógica de dibujo de fondo y elementos:
                if (cell == 'P')
                {
                    sprite.setTextureRect(wall_rect);
                    window.draw(sprite);
                }
                else 
                {
                    sprite.setTextureRect(floor_rect);
                    window.draw(sprite);

                    if (cell == 'F')
                    {
                        sprite.setTextureRect(goal_rect);
                        window.draw(sprite);
                    }
                
                    if (cell == 'I' || cell == '@')
                    {
                        sprite.setTextureRect(player_rect);
                        window.draw(sprite);
                    }
                }
            }
        }
    }
};
