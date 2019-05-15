#ifndef PUZZLE_H
#define PUZZLE_H

#include <vector>

class Element;
class Matrix;

class Puzzle
{
public:

    virtual ~Puzzle();

    void solve(bool countOnly);

protected:

    virtual void init(Matrix& matrix) = 0;
    virtual void print(std::vector<Element *>& solution) = 0;

};

#endif // !defined(PUZZLE_H)
