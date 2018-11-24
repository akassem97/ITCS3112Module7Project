#ifndef SHIP_H
#define SHIP_H


class Ship
{
    private:
    int sizeOfShip;
    //1 = North, 2 = South, 3 = East, 4 = West
    int direction;
    int x,y;

    public:

        Ship(int s, int d, int x, int y)
        {
            sizeOfShip = s;
            direction = d;
        };

        int getDirection()
        {
            return direction;
        }

        void setDirection(int d)
        {
            direction = d;
        }

        int getSize()
        {
            return sizeOfShip;
        }

        void setSize(int s)
        {
            sizeOfShip = s;
        }


    protected:


};

#endif // SHIP_H
