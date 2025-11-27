#pragma once

class Sokoban
{
    protected:

    std::vector<Box> box_positions;
      
    std::vector<Coordinate> goal_positions; 

    std::stack<GameState> Game_History;

    public:

    Sokoban(int w, int h);

    virtual void Generate_sokoban();

    bool Solve_sokoban();

    virtual bool Move_Player(char direction);

    virtual bool Check_Win();

    void Save_State();

    bool Undo_Move();

    void Generate_Initial_State();

    void Reset_Sokoban();

    bool Is_DeadLocked();
};