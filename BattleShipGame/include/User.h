#ifndef USER_H
#define USER_H

#include "Player.h"


class User : public Player
{
    public:
        User(): Player() {}
        virtual ~User() {}

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
                        if(y-i <= 0 || y-i > 10)
                            isValid = false;
                        else if(getGrid().getSquare(y, x-i) == 1)
                            isValid = false;
                    }
                    else if(d == 2)
                    {
                        if(y+i <= 0 || y+i > 10)
                            isValid = false;
                        else if(getGrid().getSquare(y, x+i) == 1)
                            isValid = false;
                    }
                    else if(d == 3)
                    {
                        if(x-i <= 0 || x-i > 10)
                            isValid = false;
                        else if(getGrid().getSquare(y-i, x) == 1)
                            isValid = false;
                    }
                    else if(d == 4)
                    {
                        if(x+i <= 0 || x+i > 10)
                            isValid = false;
                        else if(getGrid().getSquare(y+i, x) == 1)
                            isValid = false;
                    }



                    }
                }

                return isValid;
        }


    protected:

    private:
};

#endif // USER_H
