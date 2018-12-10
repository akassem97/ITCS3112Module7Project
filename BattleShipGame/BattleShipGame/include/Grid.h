#ifndef GRID_H
#define GRID_H
#include "Square.h"
using namespace std;
class Grid
{
    public:
        Grid(int p)
         {
            player = p;

            for(int i = 0; i < 10; i++)
                for(int j = 0; j < 10; j++)
                    grid[i][j] = Square(0,i,j);
         }

         Grid()
         {


            for(int i = 0; i < 10; i++)
                for(int j = 0; j < 10; j++)
                    grid[i][j] = Square(0,i,j);
         }
        void addShip(int s, int d, int x, int y)
            {
                for(int i = 0; i < s; i++)
                {

                    if(d == 1)
                            getSquare(x-1, y-i-1).setSquareValue(1);
                    else if(d == 2)
                        getSquare(x-1, y+i-1).setSquareValue(1);
                    else if(d == 3)
                        getSquare(x-i-1, y-1).setSquareValue(1);
                    else if(d == 4)
                        getSquare(x+i-1, y-1).setSquareValue(1);
                }

                cout << "Square " << getSquare(x-1, y-1).getSquareValue() << endl;
            }
         Square getSquare(int x, int y)
         {
            return grid[x][y];
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
                    s+=to_string(getSquare(i, j).getSquareValue());
                    cout << getSquare(i, j).getSquareValue();
                    s.append(" ");

                }
                cout << endl;
                s.append("\n");
                }

            //Console based UI output of grid, used for testing
            return s;
        }
    protected:

    private:
        Square grid[10][10];
        int player;
};

#endif // GRID_H
