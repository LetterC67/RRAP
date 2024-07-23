#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

const double REQUIRE_RECALCULATION = 1e18;
const double NO_MOVE = 1e9;

struct LocalSearchMove{
    double cost;
    int i, j, type;

    LocalSearchMove();
    LocalSearchMove(double cost, int i, int j, int type);
    bool operator<(const LocalSearchMove& other) const;
};

#endif