#pragma once
 
class Labyrinth
{
    protected:
    struct point
    {
        int x;
        int y;
        point* parent;
    };
    int width;

    int height;

    std::vector<std::vector<char>> map;

    int dr[4] = {-1, 1, 0, 0}; 
    int dc[4] = {0, 0, -1, 1}; 

    std::vector<point*> all_nodes;

    int player_x;
    int player_y;   

    void Clear_Nodes();

    public:

    Labyrinth(int w, int h);

    int getWidth() const;

    int getHeight() const;

    char getCell(int x, int y) const;

    virtual ~Labyrinth();

    virtual void Generate_Labyrinth();

    bool Solve_labyrinth();

    virtual bool Move_Player(char direction);

    virtual bool Check_Win();
};
