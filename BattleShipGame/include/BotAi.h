#ifndef BOTAI_H
#define BOTAI_H

#include "Player.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class BotAi : public Player
{
    public:
        BotAi(int p):Player(p) {

            //generate bot ships
            generateAiShips();



        }
        BotAi() {

            //generate bot ships
            generateAiShips();



        }

        void generateAiShips()
        {
            for(int i = 0; i < 7; )
            {
            srand (time(NULL));
            int x = rand() % 10;
            int y = rand() % 10;
            int d = rand() % 4 + 1;
            int s = rand() % 3 + 1;
            bool check = checkIfValid(x, y, d, s);
            if(check)
            {
                cout << x << " " << y << endl;
                getShips()[i] = Ship(s, d, x, y);
                i++;
                getGrid().addShip(getShips()[i]);
            }

        }
        }

        bool checkIfValid(int x, int y, int d, int s)
        {
            bool isValid = true;
            for(int i = 0; i < s; i++)
                {
                    if(d == 1)
                    {
                        if(y-i < 0 || y-i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(x-i,y).getSquareValue() == 1)
                            isValid = false;
                    }
                    else if(d == 2)
                    {
                        if(y+i < 0 || y+i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(x+i,y).getSquareValue() == 1)
                            isValid = false;
                    }
                    else if(d == 3)
                    {
                        if(x-i < 0 || x-i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(x,y-i).getSquareValue() == 1)
                            isValid = false;
                    }
                    else if(d == 4)
                    {
                        if(x+i < 0 || x+i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(x,y+i).getSquareValue() == 1)
                            isValid = false;
                    }




                }

                return isValid;
        }

        int* dropRandomBomb()
        {


        srand (time(NULL));
        int x = rand() % 10;
        int y = rand() % 10;

        int* coords = new int[2];
        coords[0] = x;
        coords[1] = y;

        if(getGrid().getSquare(x,y).getSquareValue() == 2)
        dropRandomBomb();
        else
        return coords;

        }

        int* dropBombAi()
        {
        //will make smarter using a hueristic later

        }

        virtual ~BotAi() {}

    protected:

    private:

};

#endif // BOTAI_H
