#ifndef MATRIX_H
#define MATRIX_H

#include <functional>
#include <string>
#include <vector>

#include "Header.hpp"

class Element;

class Matrix
{
public:

    Matrix();

    Header* findColumn(std::string const& name);

    int findCovers(std::function<void (std::vector<Element *>&)> print);

private:

    Header *chooseColumn();
    
    void coverColumn(Header *col);
    void uncoverColumn(Header *col);

    int search(std::vector<Element *> &solution, std::function<void (std::vector<Element *>&)> print);

    Header h;

};

#endif // !defined(MATRIX_H)
