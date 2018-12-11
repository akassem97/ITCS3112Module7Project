#ifndef GAMEMECH_H
#define GAMEMECH_H
#include "User.h"

class GameMech
{
    public:
        GameMech() {
        cout << "hallo" << endl;
        //initialize game here
        getUserInput();

        //set up grids

        //set up user game input (pvp vs pvb)

        //startGame

        }
        virtual ~GameMech() {}

        void getShipsFromUserConsole()
        {


            for(int i = 0; i < 7; )
            {

                int x,y;
                char dir;
                int d, s;
                cout << "Enter x coordinate of the face of ship" << (i+1) << endl;
                cin >> x;
                cout << "Enter y coordinate of the face of ship" << (i+1) << endl;
                cin >> y;
                cout << "Enter direction its facing (N = 1,S = 2,E = 3,W = 4)" << endl;
                cin >> d;

/*
                switch(dir)
                {
                    case 'n':
                        d = 1;
                    case 'N':
                        d = 1;
                    case 's':
                        d = 2;
                    case 'S':
                        d = 2;
                    case 'e':
                        d = 3;
                    case 'E':
                        d = 2;
                    case 'w':
                        d = 2;
                    case 'W':
                        d = 2;
                }
*/
                if(i < 3)
                    s = 2;
                else if(i < 5)
                    s = 3;
                else
                    s = 5;

                if((u.checkIfValid(y-1,x-1,d,s)))
                {

                 u.getShips()[i] = Ship(s,d,x,y);

                 u.getGrid().addShip(s,d,y,x);
                 cout << u.getGrid().toString() << endl;
                 i++;
                }
                else
                    cout << "invalid ship placement... Re Enter...";


                }


        }


        void getUserInput()
        {


            //see if user wants pvb or pvp
            //
            getShipsFromUserConsole();
            b.generateAiShips();
            //TEST FOR PvB
            cout << u.getGrid().toString();
            cout << endl << endl << endl;
            cout << b.getGrid().toString();



        }




    protected:

    private:
        Player* p1,p2;
        BotAi b;
        User u;
        bool isDone;

};

#endif // GAMEMECH_H
