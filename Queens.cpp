#include <iostream>
#include <set>
#include <limits>
#include <sstream>
#include <utility>

#include "Queens.hpp"

#include "Element.hpp"
#include "Matrix.hpp"

using namespace std;


Queens::Queens(int n)
:   n(n)
{
}


void Queens::Init(Matrix& matrix)
{
    for(int i=0; i<n; ++i) {
        for(int j=0; j<n; ++j) {
            ostringstream rname;
            rname << "R" << i;
            auto e1 = new Element();
            auto c1 = matrix.findColumn(rname.str());
            e1->InsertUD(c1);
            ostringstream fname;
            fname << "F" << j;
            auto e2 = new Element();
            auto c2 = matrix.findColumn(fname.str());
            e2->InsertUD(c2);
            e2->InsertLR(e1);
            int a = i+j;
            if (a!=0 && a!=(2*n-2)) {
                ostringstream aname;
                aname << "A" << a;
                auto e3 = new Element();
                auto c3 = matrix.findColumn(aname.str(), false);
                e3->InsertUD(c3);
                e3->InsertLR(e1);
            }
            int b = n - 1 - i + j;
            if (b!=0 && b!=(2*n-2)) {
                ostringstream bname;
                bname << "B" << b;
                auto e4 = new Element();
                auto c4 = matrix.findColumn(bname.str(), false);
                e4->InsertUD(c4);
                e4->InsertLR(e1);
            }
        }
    }
}


void Queens::Solve(bool countOnly)
{
    Matrix matrix;
    Init(matrix);
    int nodeCount = 0;
    int solutionCount = 0;
    matrix.findCovers(nodeCount, solutionCount, countOnly 
        ? function<void (vector<Element *>&)>()
        : [this](vector<Element *>& solution){ Print(solution); }
    );
    cout << solutionCount << " solutions, " << nodeCount << " nodes visted" << endl;
}


void Queens::Print(vector<Element *>& solution)
{
    static int count = 0;

    set<pair<int, int>> queens;
    for(auto& e: solution) {
        auto re = e;
        for(; re->col->name[0] != 'R'; re=re->r);
        auto r = atoi(re->col->name.c_str()+1);
        auto f = atoi(re->r->col->name.c_str()+1);
        queens.insert(pair<int, int>(r, f));
    }

    cout << "#" << ++count << ":" << endl;
    for(int f=0; f<n; ++f) {
        for(int r=0; r<n; ++r) {
            cout << (queens.count(pair<int, int>(r, f)) ? "\u265b " : "\u30fb"); 
        }
        cout << endl;
    }
    cout << endl;
}