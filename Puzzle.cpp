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
    Matrix matrix;
    init(matrix);
    int nodeCount = 0;
    int solutionCount = 0;
    matrix.findCovers(nodeCount, solutionCount, countOnly 
        ? function<void (vector<Element *>&)>()
        : [this](vector<Element *>& solution){ print(solution); }
    );
    cout << solutionCount << " solutions, " << nodeCount << " nodes visted" << endl;
}
