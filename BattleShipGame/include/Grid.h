#ifndef GRID_H
#define GRID_H


class Grid
{
    public:
        Grid(int p)
         {
            player = p;

            for(int i = 0; i < 10; i++)
                for(int j = 0; j < 10; j++)
                    grid[i][j](0,i,j);
         }

        string toString()
        {
            string s = "";

            //Console based UI output of grid, used for testing
            return s;
        }
    protected:

    private:
        private Square[10][10] grid;
        int player;
};

#endif // GRID_H
