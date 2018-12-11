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
            srand (time(NULL));
            cout << "Bot Running" << endl;
            for(int i = 0; i < 7; )
            {

            int x = rand() % 10 + 1;
            int y = rand() % 10 + 1;
            int d = rand() % 4 + 1;
            int s = rand() % 3 + 1;
            bool check = checkIfValid(y, x, d, s);
            if(check)
            {
                cout << x << " " << y << " " << d << " " << s << endl;
                getShips()[i] = Ship(s, d, x, y);
                i++;
                cout <<"adding "<< endl;
                getGrid().addShip(s,d,x,y);
            }

        }

        cout << getGrid().toString() << endl;
        }


        bool checkIfValid(int y, int x, int d, int s)
        {
            cout << "checking" << endl;
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
                        if(y-i <= 0 || y-i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(y, x-i) == 1)
                            isValid = false;
                    }
                    else if(d == 2)
                    {
                        if(y+i <= 0 || y+i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(y, x+i) == 1)
                            isValid = false;
                    }
                    else if(d == 3)
                    {
                        if(x-i <= 0 || x-i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(y-i, x) == 1)
                            isValid = false;
                    }
                    else if(d == 4)
                    {
                        if(x+i <= 0 || x+i >= 10)
                            isValid = false;
                        else if(getGrid().getSquare(y+i, x) == 1)
                            isValid = false;
                    }



                    }
                }
                cout << "end checking" << endl;
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

        if(getGrid().getSquare(x,y) == 2)
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
