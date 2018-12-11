#ifndef PLAYER_H
#define PLAYER_H
#include "Ship.h"
#include "Grid.h"
class Player
{
    public:
        Player(int p) {
            playerNum = p;
            shipCount = 0;
            score = 0;
            ships = new Ship[7];
            g = Grid();
        }
        Player()
        {
            shipCount = 0;
            score = 0;
            ships = new Ship[7];
            g = Grid();
        }
        virtual ~Player() {}
        Ship* getShips()
        {
            return ships;
        }

        void setShips(Ship* s)
        {
            ships = s;
        }

        Grid getGrid()
        {
            return g;
        }

        bool didLose()
        {
            int counter = 0;
            for(int i = 0; i < 10; i++)
                for(int j = 0; j < 10; j++)
                    if(getGrid().getSquare(i,j) == 1)
                        counter++;
            if(counter == 0)
            return true;
            else
            return false;
        }
    protected:

    private:
        Ship* ships;
        int playerNum;
        int shipCount;
        int score;
        Grid g;

};

#endif // PLAYER_H
