#ifndef GRID_H
#define GRID_H
#include "Square.h"
#include <iostream>
using namespace std;
class Grid
{
    private:
        //Square** grid = new Square*[10];
        //Square** grid = new Square*[10];
        int** grid = new int*[10];
        int player;

    public:

        /**
        * Grid constructor that instantiates a players grid and add ships to the grid.
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param none
        *@return playerNum int
        */
        Grid(int p)
         {

            for(int i = 0; i < 10; i++)
                grid[i] = new int[10];


            player = p;

            for(int i = 0; i < 10; i++)
                for(int j = 0; j < 10; j++)
                    grid[i][j] = 0;

                    //cout << "it worked" << endl;
         }

         /**
        * Grid constructor that instantiates a players grid which adds ships to the grid.
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param none
        *@return none
        */
         Grid()
         {

            for(int i = 0; i < 10; i++)
                grid[i] = new int[10];
            for(int i = 0; i < 10; i++)
                for(int j = 0; j < 10; j++)
                    grid[i][j] = 0;
         }

         /**
        * Adds a ship to the grid
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param size int
        *@param direction int
        *@param y coordinate int
        *@param x coordinate int
        *@return none
        */
        void addShip(int s, int d, int y, int x)
            {
                for(int i = 0; i < s; i++)
                {

                    if(d == 1)
                        //getSquare(x-1, y-i-1).setSquareValue(1);
                        grid[y+i-1][x-1] = 1;
                    else if(d == 2)
                        grid[y-i-1][x-1] = 1;
                    else if(d == 3)
                        grid[y-1][x-1-i] = 1;
                    else if(d == 4)
                        grid[y-1][x-1+i] = 1;
                }


            }
        /**
        * gets the square value of a specified coordinate in the grid.
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param y coordinate int
        *@param x coordinate int
        *@return none
        */
         int getSquare(int y, int x)
         {
            return grid[y][x];
         }
        /**
        * If a part of a ship is hit, this method sets the square value to hit aka 2.
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param y coordinate int
        *@param x coordinate int
        *@return none
        */
         void hit(int x, int y)
         {
            grid[y-1][x-1] = 2;
         }
        /**
        * Drops a bomb on this grid
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param y coordinate int
        *@param x coordinate int
        *@return outcome string (returns a string representation of outcome of this move "hit" or "miss")
        */
         string dropBomb(int x, int y)
         {
            string s;
            if(getSquare(y-1,x-1) == 1)
            {
                hit(x,y);
                s = "Hit!";

            }
            else
            s = "Miss..";

            return s;
         }
        /**
        * Drops a bomb on this grid
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param none
        *@return string (returns a string representation of the grid for visualization)
        */
        string toString()
        {
            string s = "";

            for(int i = 0; i < 10; i++)
            {
                for(int j = 0; j < 10; j++)
                {
                    s.append("| ");
                   // s.append(getSquare(i, j).getSquareValue());
                    s+=to_string(grid[i][j]);
                    //cout << grid[i][j];
                    s.append(" ");

                }
                //cout << endl;
                s.append("\n");
                }

            //Console based UI output of grid, used for testing
            return s;
        }
    protected:


};

#endif // GRID_H
