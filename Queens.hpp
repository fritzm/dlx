#ifndef QUEENS_H
#define QUEENS_H

#include <vector>

class Element;
class Matrix;

class Queens
{
public:

    Queens(int n);

    void Solve(bool countOnly);

private:

    void Init(Matrix& matrix);
    void Print(std::vector<Element *>& solution);

    int n;

};

#endif // !defined(QUEENS_H)
