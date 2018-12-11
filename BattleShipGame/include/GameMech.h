#ifndef GAMEMECH_H
#define GAMEMECH_H
#include "User.h"
#include "BotAi.h"
#include <iostream>
using namespace std;
class GameMech
{
    public:
        GameMech() {

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
                cout << "Enter x coordinate of the face of ship " << (i+1) << endl;
                cin >> x;
                cout << "Enter y coordinate of the face of ship " << (i+1) << endl;
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

                if((u.checkIfValid(y,x,d,s)))
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

        void startPvp(){
        //launch client and connect to server

        //get user input
            getShipsFromUserConsole();

        //game loop!

        //wait for server instruction (either your turn or wait)

        //if wait, keep listening, if play then execute function where use selects a place to drop bomb

        //once turn is over, send coordinates to server and go back to listening
        }

        void startPvBot()
        {
            b.generateAiShips();
            getShipsFromUserConsole();

            while(!u.didLose() || !b.didLose())
            {
                int x,y;
                do
                {
                cout << "Your turn, type in coordinates to drop bomb\nEnter x:" << endl;
                cin >> x;
                cout << "Enter y: " << endl;
                cin >> y;
                if(x <= 0 || x > 10 || y <= 0 || y > 10)
                cout << "Invalid Entry.. Re Enter" << endl;
                }
                while(x <= 0 || x > 10 || y <= 0 || y > 10);

                string s = b.getGrid().dropBomb(x,y);

                cout << "It was a " << s << "\nBots turn..." << endl;

                int* coords = b.dropRandomBomb();

                s = u.getGrid().dropBomb(coords[0], coords[1]);

                cout << "It was a " << s << endl;

                cout  << "Your board:\n" << u.getGrid().toString() << endl;



            }



        }
        void getUserInput()
        {


            //see if user wants pvb or pvp
            cout << "Welcome to Battleship!\nDo you want to play 1. PvP or 2. PVBot?" << endl;
            int choice = 0;
            while(choice != 2 && choice != 1)
            {
            cin >> choice;
            switch(choice){
            case 1:
            startPvp();
            break;
            case 2:
            startPvBot();
            break;
            default:
            cout << "Invalid choice... Re Enter" << endl;
            }
            }
            //


            //TEST FOR PvB
            //cout << u.getGrid().toString();
            //cout << endl << endl << endl;
            //cout << b.getGrid().toString();



        }




    protected:

    private:
        Player* p1,p2;
        BotAi b;
        User u, o;
        bool isDone;

};

#endif // GAMEMECH_H
