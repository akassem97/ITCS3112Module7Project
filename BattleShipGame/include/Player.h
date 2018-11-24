#ifndef PLAYER_H
#define PLAYER_H


class Player
{
    public:
        Player(int p) {
            player = p;
            shipCount = 0;
            score = 0;
            Ship = new Ship[7];
        }
        virtual ~Player() {}
        Ship* getShips()
        {
            return ships;
        }

        void setShips(Ship* s)
        {
            ships = s;
        }

        Grid getGrid()
        {
            return g;
        }
    protected:

    private:
        Ship* ships;
        int playerNum;
        int shipCount;
        int score;
        Grid g;

};

#endif // PLAYER_H
