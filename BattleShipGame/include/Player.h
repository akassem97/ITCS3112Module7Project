#ifndef PLAYER_H
#define PLAYER_H


class Player
{
    public:
        Player(int p) {
            player = p;
            shipCount = 0;
            score = 0;
        }
        virtual ~Player() {}

    protected:

    private:
        Ship[7] ships;
        int playerNum;
        int shipCount;
        int score;

};

#endif // PLAYER_H
