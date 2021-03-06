#ifndef GRID_H
#define GRID_H
#include "Square.h"
#include <iostream>
using namespace std;
/**
* This class implements the grid and grid functionalities used in game
*@author Adham Kassem
*@author Jonathon Henly
*/
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
         * Returns a flat stream of unsigned char values that defines the grid state
         *@author Adham Kassem
         *@author Jonathon Henly
         *@param none
         *@return stream unsigned char*
         */
         unsigned char* getGridStream()
         {
            unsigned char* stream = new unsigned char[100];

            for(int i = 0; i < 10; i++)
                for(int j = 0; j < 10; j++)
                    {
                    unsigned char c;
                    if(grid[i][j] == 1)
                    c = '2';
                    else if(grid[i][j] == 0)
                    c = '1';
                    else
                    c = '3';


                    stream[i*10+j] = c;
                    }

                    return stream;

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
            string s = "  1   2   3   4   5   6   7   8   9   10\n";

            for(int i = 0; i < 10; i++)
            {
                for(int j = 0; j < 10; j++)
                {
                    s.append("| ");
                   // s.append(getSquare(i, j).getSquareValue());
                    if(grid[i][j] == 0)
                    s.append(" ");
                    else if(grid[i][j] == 1)
                    s.append("'");
                    else
                    s.append("X");
                    //cout << grid[i][j];
                    s.append(" ");

                }
                //cout << endl;
                s.append(" ");
                s.append(to_string(i+1));
                s.append("\n");
                }

            //Console based UI output of grid, used for testing
            return s;
        }

    /**
    *This method is meant for user objects that represent the opponent user locally. When the server informs client that the clients turn was a hit, this method will change the local
    *state of the grid for proper local visualization
    *@author Adham Kassem
    *@author Jonathon Henly
    *@param x int
    *@param y int
    *@return none
    */
        void addHit(int x, int y)
        {
            grid[y][x] = 2;
        }
    protected:


};

#endif // GRID_H
