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
    init();

    int nodeCount = 0, solutionCount = 0;
    for(auto& sg: subGoals) {
        sg.matrix.findCovers(sg.nodes, sg.solutions, countOnly 
            ? function<void (vector<Element *>&)>()
            : [this](vector<Element *>& solution){ print(solution); }
        );
        solutionCount += sg.solutions;
    }

    cout << solutionCount << " solutions found" << endl << endl;

    if (!subGoals.empty()) {

        int primaryCols, totalCols;
        subGoals[0].matrix.getColumnStats(primaryCols, totalCols);
        cout << totalCols << " constraint columns (" << primaryCols << " primary)" << endl;

        int n = 0;
        for(auto& sg: subGoals) {
            if (subGoals.size() > 1) cout << "Subgoal " << ++n << ": ";
            cout << sg.rows << " rows, " << sg.elems << " elements, ";
            if (subGoals.size() > 1) cout << sg.solutions << " solutions, ";
            cout << sg.nodes << " nodes traversed" << endl;
        }

        cout << endl;
    }

}
