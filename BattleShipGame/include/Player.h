#ifndef PLAYER_H
#define PLAYER_H
#include "Ship.h"
#include "Grid.h"
class Player
{
    private:
        Ship* ships;
        int playerNum;
        int shipCount;
        int score;
        Grid g;


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
        /**
        * Checks to see if ship placement is valid
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param y coordinate int
        *@param x coordinate int
        *@param d direction int
        *@param s size int
        *@return isValid boolean (returns whether or not the placement is valid)
        */
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
        /**
        * setter for ships
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param s Ship
        *@return none
        */
        void setShips(Ship* s)
        {
            ships = s;
        }

        /**
        * getter for grid
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param none
        *@return g Grid
        */
        Grid getGrid()
        {
            return g;
        }

        /**
        * setter for grid
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param g1 Grid
        *@return none
        */
        void setGrid(Grid g1)
        {
            g = g1;
        }

        /**
        * Returns a boolean value on whether or not the player lost.
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param none
        *@return didLose boolean
        */
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



};

#endif // PLAYER_H
