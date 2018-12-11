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
         bool checkIfValid(int y, int x, int d, int s)
        {
            //cout << "checking" << endl;
            bool isValid = true;
            if(y <= 0 || y >= 10 || x <= 0 || x >= 10)
                isValid = false;
            else if(d < 1 || d > 4)
                isValid = false;
            else
            for(int i = 0; i < s; i++)
                {
                if(isValid == true)
                {
                    if(d == 1)
                    {
                        if(y+i <= 0 || y+i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(y+i, x) == 1)
                            isValid = false;
                    }
                    else if(d == 2)
                    {
                        if(y-i <= 0 || y-i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(y-i, x) == 1)
                            isValid = false;
                    }
                    else if(d == 3)
                    {
                        if(x-i <= 0 || x-i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(y, x-i) == 1)
                            isValid = false;
                    }
                    else if(d == 4)
                    {
                        if(x+i <= 0 || x+i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(y, x+i) == 1)
                            isValid = false;
                    }



                    }
                }
                //cout << "end checking" << endl;
                return isValid;
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
