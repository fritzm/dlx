#include <iostream>
#include <vector>

#include "Puzzle.hpp"

#include "Matrix.hpp"

using namespace std;


Puzzle::~Puzzle()
{
}


void Puzzle::solve(bool countOnly)
{
    int rows = 0, primaryCols = 0, totalCols = 0, elems = 0;
    init(rows, primaryCols, totalCols, elems);

    int nodeCount = 0, solutionCount = 0;
    for(auto &matrix: subGoals) {
        matrix.findCovers(nodeCount, solutionCount, countOnly 
            ? function<void (vector<Element *>&)>()
            : [this](vector<Element *>& solution){ print(solution); }
        );
    }

    cout << subGoals.size() << " subgoals, ";
    cout << rows << "x" << totalCols << " constraint matrix, ";
    cout << primaryCols << " primary columns, ";
    cout << elems << " elements" << endl;
    cout << solutionCount << " solutions, ";
    cout << nodeCount << " search nodes visted" << endl;
}
