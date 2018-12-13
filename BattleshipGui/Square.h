#ifndef SQUARE_H
#define SQUARE_H

/**
* This class stores information for each square in a grid
*@author Adham Kassem
*@author Jonathon Henly
*/
class Square
{
    public:
         /**
        * Constructor for a Square object
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param squareVal int
        *@param x int
        *@param y int
        */
        Square(int s, int x1, int y1) {
            squareValue = s;
            x = x1;
            y = y1;
        }
        /**
        * Constructor for a Square object
        *@author Adham Kassem
        *@author Jonathon Henly
        */
        Square()
        {
            squareValue = 0;
        }
        /**
        * getter for square value
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param none
        *@return squareVal int
        */
        int getSquareValue()
        {
            return squareValue;
        }

        /**
        * setter for square value
        *@author Adham Kassem
        *@author Jonathon Henly
        *@param squareVal int
        *@return none
        */
        void setSquareValue(int s)
        {

            squareValue = s;
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
        *@param x int
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
        *@param y int
        *@return none
        */
        void setY(int y1)
        {
            y = y1;
        }

    protected:

    private:
        int squareValue;
        int x,y;
        char xc,yc;
};

#endif // SQUARE_H
