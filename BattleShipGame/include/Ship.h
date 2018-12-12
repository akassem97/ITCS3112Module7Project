#ifndef SHIP_H
#define SHIP_H
using namespace std;

/**
* This class stores Ship info for each ship
*@author Adham Kassem
*@author Jonathon Henly
*/
class Ship
{
    private:
    int sizeOfShip;
    //1 = North, 2 = South, 3 = East, 4 = West
    int direction;
    int x,y;

    public:
        /**
        * Constructor for a Ship object
        *@author Adham Kassem
        *@author Jonathon Henly
        */
        Ship()
        {

        }
        /**
        * Constructor for a Ship object
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param y coordinate int
        *@param x coordinate int
        *@param d direction int
        *@param s size int
        */
        Ship(int s, int d, int x1, int y1)
        {

            sizeOfShip = s;
            direction = d;
            x = x1;
            y = y1;

        }

        /**
        * getter for direction
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param none
        *@return direction int
        */
        int getDirection()
        {
            return direction;
        }

        /**
        * setter for direction
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param d int
        *@return none
        */
        void setDirection(int d)
        {
            direction = d;
        }

        /**
        * getter for size
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param none
        *@return size int
        */
        int getSize()
        {
            return sizeOfShip;
        }
        /**
        * setter for size
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param size int
        *@return none
        */
        void setSize(int s)
        {
            sizeOfShip = s;
        }

        /**
        * getter for x coordinate
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param none
        *@return x int
        */
        int getX()
        {
            return x;
        }

        /**
        * setter for x coordinate
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param x1 int
        *@return none
        */
        void setX(int x1)
        {
            x = x1;
        }

        /**
        * getter for y coordinate
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param none
        *@return y int
        */
        int getY()
        {
            return y;
        }

        /**
        * setter for y coordinate
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param y1 int
        *@return none
        */
        void setY(int y1)
        {
            y = y1;
        }


    protected:


};

#endif // SHIP_H
