#ifndef SHIP_H
#define SHIP_H
using namespace std;

class Ship
{
    private:
    int sizeOfShip;
    //1 = North, 2 = South, 3 = East, 4 = West
    int direction;
    int x,y;

    public:
        Ship()
        {

        }
        Ship(int s, int d, int x1, int y1)
        {

            sizeOfShip = s;
            direction = d;
            x = x1;
            y = y1;

        }

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

        int getX()
        {
            return x;
        }

        int getY()
        {
            return y;
        }


    protected:


};

#endif // SHIP_H
