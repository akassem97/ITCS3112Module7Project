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
            //cout << "Bot Running" << endl;
            for(int i = 0; i < 7; )
            {

            int x = rand() % 10 + 1;
            int y = rand() % 10 + 1;
            int d = rand() % 4 + 1;
            int s = rand() % 3 + 1;
            bool check = checkIfValid(y, x, d, s);
            if(check)
            {
                //cout << x << " " << y << " " << d << " " << s << endl;
                getShips()[i] = Ship(s, d, x, y);
                i++;
                //cout <<"adding "<< endl;
                getGrid().addShip(s,d,y,x);
            }

        }

        //cout << getGrid().toString() << endl;
        }




        int* dropRandomBomb()
        {


        srand (time(NULL));
        int x = rand() % 10 + 1;
        int y = rand() % 10 + 1;

        int* coords = new int[2];
        coords[0] = x;
        coords[1] = y;
        cout << x << " " << y << endl;

        if(getGrid().getSquare(x-1,y-1) == 2)
        coords = dropRandomBomb();

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
