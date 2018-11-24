#ifndef SHIP_H
#define SHIP_H


class Ship
{
    private:
    int sizeOfShip;
    //1 = North, 2 = South, 3 = East, 4 = West
    int direction;

    public:

        Ship(int s, int d)
        {
            sizeOfShip = s;
            direction = d;
        };


    protected:


};

#endif // SHIP_H
