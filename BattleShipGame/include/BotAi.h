#ifndef BOTAI_H
#define BOTAI_H

#include <Player.h>


class BotAi : public Player
{
    public:
        BotAi(int p) {
            Player(p);

            //generate bot ships
            generateAiShips();
        }

        void generateAiShips()
        {

        }
        virtual ~BotAi() {}

    protected:

    private:
};

#endif // BOTAI_H
