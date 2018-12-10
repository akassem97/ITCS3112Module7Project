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
        void addShip(Ship s)
            {
                for(int i = 0; i < s.getSize(); i++)
                {
                    if(s.getDirection() == 1)
                            getSquare(s.getX()-1, s.getY()-i-1).setSquareValue(1);
                    else if(s.getDirection() == 2)
                        getSquare(s.getX()-1, s.getY()+i-1).setSquareValue(1);
                    else if(s.getDirection() == 3)
                        getSquare(s.getX()-i-1, s.getY()-1).setSquareValue(1);
                    else if(s.getDirection() == 4)
                        getSquare(s.getX()+i-1, s.getY()-1).setSquareValue(1);
                }
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
