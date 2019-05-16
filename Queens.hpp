#ifndef QUEENS_H
#define QUEENS_H

#include <vector>

#include "Puzzle.hpp"

class Element;
class Matrix;

class Queens : public Puzzle
{
public:

    Queens(int n);

private:

    void init(int& rows, int& primaryCols, int& totalCols, int& elem) override;
    void print(std::vector<Element *>& solution) override;

    int n;

};

#endif // !defined(QUEENS_H)
