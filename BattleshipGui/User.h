#ifndef USER_H
#define USER_H

#include "Player.h"

/**
* This class inherits all Player functionalities but is specified towards a user, not a bot.
*@author Adham Kassem
*@author Jonathon Henly
*/
class User : public Player
{
    public:
        User(): Player() {}
        virtual ~User() {}

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
        getGrid().addHit(x-1,y-1);
    }



    protected:

    private:
};

#endif // USER_H
