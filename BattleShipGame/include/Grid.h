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

         Grid()
         {

            for(int i = 0; i < 10; i++)
                grid[i] = new int[10];
            for(int i = 0; i < 10; i++)
                for(int j = 0; j < 10; j++)
                    grid[i][j] = 0;
         }
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
         int getSquare(int y, int x)
         {
            return grid[y][x];
         }

         void hit(int x, int y)
         {
            grid[y-1][x-1] = 2;
         }

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
