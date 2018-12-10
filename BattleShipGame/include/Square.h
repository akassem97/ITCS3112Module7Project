#ifndef SQUARE_H
#define SQUARE_H


class Square
{
    public:
        Square(int s, int x1, int y1) {
            squareValue = s;
            x = x1;
            y = y1;
        }

        Square()
        {

        }

        int getSquareValue()
        {
            return squareValue;
        }

        void setSquareValue(int s)
        {
            squareValue = s;
        }

        int getX()
        {
            return x;
        }

        void setX(int x1)
        {
            x = x1;
        }

        int getY()
        {
            return y;
        }

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
