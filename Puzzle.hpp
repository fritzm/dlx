#ifndef PUZZLE_H
#define PUZZLE_H

#include <vector>

#include "Matrix.hpp"

class Element;

class Puzzle
{
public:

    virtual ~Puzzle();

    void solve(bool countOnly);

protected:

    virtual void init() = 0;
    virtual void print(std::vector<Element *>& solution) = 0;

    struct SubGoal {
        Matrix matrix;
        int rows = 0;
        int elems = 0;
        int nodes = 0;
        int solutions = 0;
    };

    std::vector<SubGoal> subGoals;

};

#endif // !defined(PUZZLE_H)
