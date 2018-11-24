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
        virtual ~Square() {}

    protected:

    private:
        int squareValue;
        int x,y;
};

#endif // SQUARE_H
