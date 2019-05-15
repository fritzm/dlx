#ifndef QUEENS_H
#define QUEENS_H

#include <vector>

class Element;
class Matrix;

class Queens
{
public:

    Queens(int n);

    void solve(bool countOnly);

private:

    void init(Matrix& matrix);
    void print(std::vector<Element *>& solution);

    int n;

};

#endif // !defined(QUEENS_H)
