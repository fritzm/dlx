#ifndef MATRIX_H
#define MATRIX_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "Element.hpp"


class Matrix
{
public:

    Matrix();

    Header* findColumn(std::string const& name, bool isPrimary=true);

    void findCovers(
        int &nodeCount,
        int &solutionCount,
        std::function<void (std::vector<Element *>&)> print
    );

private:

    Header *chooseColumn();
    
    void coverColumn(Header *col);
    void uncoverColumn(Header *col);

    void search(
        int &nodeCount,
        int &solnCount,    
        std::vector<Element *> &solution, 
        std::function<void (std::vector<Element *>&)> print
    );

    Header h;
    std::map<std::string, Header*> columns;

};

#endif // !defined(MATRIX_H)
