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

    virtual void init(int& rows, int& primaryCols, int& totalCols, int& elems) = 0;
    virtual void print(std::vector<Element *>& solution) = 0;

    std::vector<Matrix> subGoals;

};

#endif // !defined(PUZZLE_H)
